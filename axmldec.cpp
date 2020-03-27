#include "axmldec.hpp"
#include "jitana/util/axml_parser.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/property_tree/xml_parser.hpp>

struct membuf : std::streambuf {
    membuf(const char* base, size_t size)
    {
        char* p(const_cast<char*>(base));
        this->setg(p, p, p + size);
    }
};

struct imemstream : virtual membuf, std::istream {
    imemstream(char const* base, size_t size)
            : membuf(base, size),
              std::istream(static_cast<std::streambuf*>(this))
    {
    }
};

void write_xml(std::ostream& os, const boost_pt::ptree& pt)
{
    // Write the ptree to the output.
    {
#if BOOST_MAJOR_VERSION == 1 && BOOST_MINOR_VERSION < 56
        boost_pt::xml_writer_settings<char> settings(' ', 2);
#else
        boost_pt::xml_writer_settings<std::string> settings(' ', 2);
#endif
        boost_pt::write_xml(os, pt, settings);
    }
}

void process_content(std::istream& input_content,
                     std::ostream& output_content)
{
    // Property tree for storing the XML content.
    boost_pt::ptree pt;

    // Load the XML into ptree.
    if (input_content.peek() == 'P') {
        std::cerr << "android apk binary is not supported";
        return;
    }
    else if (input_content.peek() == 0x03) {
        jitana::read_axml(input_content, pt);
    }
    else {
        boost_pt::read_xml(input_content, pt, boost_pt::xml_parser::trim_whitespace);
    }

    // Write the tree as an XML file.
    write_xml(output_content, pt);
}


void process_file(const std::string& input_filename,
                  const std::string& output_filename)
{
    std::ifstream ifs(input_filename, std::ios::binary);

    // Construct the output stream.
    std::ostream* os = &std::cout;
    std::ofstream ofs;
    if (!output_filename.empty()) {
        ofs.open(output_filename);
        os = &ofs;
    }

    process_content(ifs, *os);
}

std::string process_bytes(char const* input_content, size_t input_content_size)
{
    imemstream ims(input_content, input_content_size);
    std::ostringstream ostrs;

    process_content(ims, ostrs);

    return ostrs.str();
}

