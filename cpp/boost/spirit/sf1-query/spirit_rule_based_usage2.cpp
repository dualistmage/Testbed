#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <boost/spirit.hpp>
#include <boost/spirit/core.hpp>
#include <boost/function.hpp>

enum status_e
{
  SIGNOUT,
  BUSY,
  AVAILABLE  
};

std::string status2str(status_e sts)
{
  switch (sts) {
  case SIGNOUT:   return "SIGNOUT";
  case BUSY:      return "BUSY";
  case AVAILABLE: return "AVAILABLE";
  }

  return "";
}

struct contact_t
{
  std::string id;
  std::string name;
  std::string email;
  std::string ip4;
  status_e    sts;
};

struct debug_s
{
  template <typename IteratorT>
  void operator()(IteratorT first, IteratorT last) const {
    std::cout << '\"' << std::string(first, last) <<  '\"' << std::endl;
  }
};

struct statusST : boost::spirit::symbols<status_e>
{
  statusST()
  {
    add
      ("SIGNOUT"   , SIGNOUT)
      ("BUSY"      , BUSY)
      ("AVAILABLE" , AVAILABLE)
      ("S/O"       , SIGNOUT)
      ("B/S"       , BUSY)
      ("A/V"       , AVAILABLE)
      ;
  }
};


template <class Action>
struct contact_list : public boost::spirit::grammar<contact_list<Action> >
{
  contact_list(Action& action_) : action(action_)
  {
  }

  template <typename ScannerT>
  struct definition
  {
    definition(contact_list const& self)
    {
      using namespace boost::spirit;

      Action& action = self.action;

      port         = ch_p(':') >> +digit_p;
      ip4          = +digit_p >> '.' >> +digit_p >> '.' >> +digit_p >> '.' >> +digit_p >> !port;
      domainName   = ip4 | domain;
      domain       = nameString;
      nameString   = alpha_p >> *(alnum_p | '-' | '.');
      quotedString = confix_p('"', (*c_escape_ch_p), '"');
      name         = quotedString;
      email        = nameString >> '@' >> domainName;
      id           = nameString;
      contact      = id[action.id] >> '=' >> confix_p('{', email[action.email] >> ',' >> name[action.name] >> ',' >> status_p[action.status] >> ',' >> !ip4[action.ip4], '}') >> ';';
      contacts     = *contact[action.contact];
    }


    statusST status_p;

    boost::spirit::rule<ScannerT> contacts, contact, id, email, name, quotedString, nameString, domain, domainName, ip4, port;

    boost::spirit::rule<ScannerT> const& start() const {
      return contacts;
    }
  };

  Action& action;
};

struct contact_callback
{
  typedef boost::function1<void, const contact_t&> callback_t;

  contact_callback(callback_t cb_) : cb(cb_),
				     id(data.id),
				     email(data.email),
				     name(data.name),
				     ip4(data.ip4),
				     status(data.sts),
				     contact(cb, data) {
  }

  struct assign_a
  {
    assign_a(std::string& ref_) : ref(ref_) {
    }

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
      ref = std::string(first, last);
    }

    std::string& ref;
  };

  struct status_a
  {
    status_a(status_e& ref_) : ref(ref_) {
    }

    void operator()(status_e sts) const {
      ref = sts;
    }

    status_e& ref;
  };

  struct contact_a
  {
    contact_a(callback_t cb_, contact_t& data_) : cb(cb_), data(data_) {
    }

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
      cb(data);
    }
   
    callback_t cb;
    contact_t& data;
  };

  callback_t cb;

  contact_t data;

  assign_a id, email, name, ip4;
  status_a status;
  contact_a contact;
};

struct callback
{
  void operator()(const contact_t& ct) {
    std::cout << ct.id << " = {\n\t"
	      << ct.email << ",\n\t" 
	      << ct.name << ",\n\t"
	      << status2str(ct.sts) << ",\n\t"
	      << ct.ip4 << "\n"
	      << "}\n";
  }
};

bool do_parse(const char* buff)
{
  using namespace std;
  using namespace boost::spirit;

  contact_callback::callback_t f = callback();
  contact_callback action(f);
  contact_list<contact_callback> parser(action);
  parse_info<> info = parse(buff, parser, space_p);
  if (!info.full) {
    cout << ">> parse error" << endl;
    cout << info.full << endl;
    cout << info.hit << endl;
    cout << info.length << endl;
    cout << info.stop << endl;
  }

  return info.full;
}

int main()
{
  using namespace std;

  ifstream infile;
  infile.open("zz.txt", ifstream::in);

  std::string buff;
  while (infile.good()) {
    char c = infile.get();
    if (infile.eof()) break;
    buff += c;
  }

  infile.close();

  do_parse(buff.c_str());
}
