#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <algorithm>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;
using std::ifstream;

bool is_valid_char(const char& c);
string fileAsString(ifstream& input);
unordered_set<string> findWikiLinks(const string& page_html);

int main() {
	
    /* TODO: Write code here! */

    /* Note if your file reading isn't working, please follow the
     * steps posted on the front page of the CS106L website. Essentially
     * go to the projects tab on the panel on the left, and in the run
     * section, uncheck the "Run in terminal" box and re-check it. This
     * should fix things.
     */

    // change the harcoded files by prompting user for file name
    ifstream file("simple.txt");

    string file_str = fileAsString(file);

    unordered_set<string> links = findWikiLinks(file_str);
    //cout << "There are " << links.size();
    //cout << " valid links: " << endl;
    //for(string link:links) {
    //    cout << link << endl;
    //}
    //cout << endl;
}

bool is_valid_char(const char& c) {
    return ((c != '#') && (c != ':'));
}

string fileAsString(ifstream& input) {
    string result;
    string line;
    while(std::getline(input, line)) {
        result += line;
    }
    return result;
}

//unordered_set<string> findWikiLinks(const string& page_html)
unordered_set<string> findWikiLinks(const string& page_html) {
    unordered_set<string> links;
    /*
     * start and end denotes the ranges of the page_html that we
     * should search for the links within.
     */
    auto start = page_html.begin();
    auto end = page_html.end();

    /*
     * pre_link_str denotes the string of text that
     * comes before a link in an html file.
     */
    const string pre_link_str = "<a href=\"/wiki/";

    while(start != end) {
        /* Find occurence of links in the range [start, end).
         *
         * The std::search algorithm looks for the sequence denoted
         * by [pre_link_str.begin(), pre_link_str.end()) and returns an
         * iterator to the position where the sequence was found in the
         * text from [start, end).
         *
         * If the sequence isn't found, the algorithm returns the end iterator.
         *
         */
        auto pre_link_itr = std::search(start, end, pre_link_str.begin(), pre_link_str.end());

        /* If no sequence of feature found in the search range,
         * there is nothing left to do.
         */
        if(pre_link_itr == end) {
            break;
        }
        else {
            // this is an iterator to the start position of the link
            auto link_start_itr = pre_link_itr + pre_link_str.length();
            const char post_link_char = '\"';
            /*
             * It returns the iterator to the position where
             * the link end, i,e the position of " after <a href=
             * If " isn't found, the algorithm returns the end iterator.
             */
            auto post_link_itr = std::find(link_start_itr, end, post_link_char);
            //auto post_link_itr = std::search(pre_link_itr, end, post_link_str.begin(), post_link_str.end());
            if(post_link_itr == end) {
                cout << "link is not valid" << endl;
                break;
            }
            else {
                string link;
                std::copy(link_start_itr, post_link_itr,
                              std::back_inserter(link));
                // filter invalid links
                if(std::all_of(link.begin(), link.end(), is_valid_char))
                    links.insert(link);
                start = post_link_itr;
            }
        }
    }
    return links;
}

