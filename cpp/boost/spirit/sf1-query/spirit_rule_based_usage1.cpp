#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <boost/spirit.hpp>
#include <boost/spirit/core.hpp>

enum status_e
{
  SIGNOUT,
  BUSY,
  AVAILABLE  
};

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

      status       = +alpha_p;
      port         = ch_p(':') >> +digit_p;
      ip4          = +digit_p >> '.' >> +digit_p >> '.' >> +digit_p >> '.' >> +digit_p >> !port;
      domainName   = ip4 | domain;
      domain       = nameString;
      nameString   = alpha_p >> *(alnum_p | '-' | '.');
      quotedString = confix_p('"', (*c_escape_ch_p), '"');
      name         = quotedString;
      email        = nameString >> '@' >> domainName;
      id           = nameString;
      contact      = id[action.id] >> '=' >> confix_p('{', email[action.email] >> ',' >> name[action.name] >> ',' >> status[action.status] >> ',' >> !ip4[action.ip4], '}') >> ';';
      contacts     = *contact[action.contact];
    }

    boost::spirit::rule<ScannerT> contacts, contact, id, email, name, quotedString, nameString, domain, domainName, ip4, port, status;

    boost::spirit::rule<ScannerT> const& start() const {
      return contacts;
    }
  };

  Action& action;
};

struct beautifier
{
  struct id_a
  {
    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
      std::cout << std::string(first, last) << " = {\n";
    }
  } id;

  struct comma_a
  {
    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
      std::cout << '\t' << std::string(first, last) << ",\n";
    }
  } email, name, status, ip4;

  struct contact_a
  {
    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const {
      std::cout << "};\n";
    }
  } contact;
};

bool do_parse(const char* buff)
{
  using namespace std;
  using namespace boost::spirit;

  beautifier action;
  contact_list<beautifier> parser(action);
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
  infile.open("test.txt", ifstream::in);

  std::string buff;
  while (infile.good()) {
    char c = infile.get();
    if (infile.eof()) {
      break;
    }
    buff += c;
  }

  infile.close();

  do_parse(buff.c_str());
}
