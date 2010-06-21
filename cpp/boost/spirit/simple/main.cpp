///
/// @brief Simple test code of boost::spirit
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010.06.21
///

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/include/classic.hpp>

void testCall(char const*, char const*)
{
    std::cout << "Test Call " << std::endl;
}

bool do_parse(const char* line, std::map<std::string, std::vector<int> >& result)
{
    using namespace std;
    using namespace boost::spirit::classic;
    std::string name;
    std::vector<int> array;

    parse_info<> info = parse(line,
            (
             (eps_p)[&testCall]
             >> (+alnum_p)[assign_a(name)]
             >> '='
             >> int_p[push_back_a(array)]
             >> *( ch_p(',') >> int_p[push_back_a(array)])
             >> ';'
            ),
            space_p
            );

    
    if ( info.full )
    {
        cerr << "Mid REsult(" << name << ") : ";
        for(size_t i = 0;i < array.size(); i++)
            cerr << array[i] << " ";
        cerr << endl;

        result[name] = array;

        std::map<std::string, std::vector<int> >::iterator iter = result.find(name);
        if ( iter != result.end() )
        {
            cerr << "Mid REsult2(" << iter->first << ") : ";
            for(size_t i = 0;i < iter->second.size(); i++)
                cerr << iter->second[i] << " ";
            cerr << endl;
        }
    }

    return info.full;
} // end - do_parse()

int main()
{
    using namespace std;

    const int MAXLINE = 1024;

    ifstream is;
    is.open("test.txt", ifstream::in);

    char buff[MAXLINE];
    std::map<std::string, vector<int> > result;
    for (int i = 1; is.good(); ++i) {
        is.getline(buff, sizeof(buff));
        if ( !is.good() )
            break;

        if (!do_parse(buff, result)) {
            cout << "line: " << i << " - parse error. buff is \"" << buff << "\"" << endl;
        }
    }

    for(std::map<std::string, vector<int> >::iterator iter = result.begin();
            iter != result.end(); iter++)
    {
        cout << "Key(" << iter->first << ") : ";
        for(size_t i = 0; i < iter->second.size(); i++)
        {
            cout << iter->second[i];
            if ( i != iter->second.size() - 1 )
                cout << ", ";
        }
        cout << endl;
    }



    is.close();
}
