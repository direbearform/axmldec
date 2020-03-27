#include <iostream>
#include <string>

#include <boost/property_tree/ptree.hpp>

namespace boost_pt = boost::property_tree;

void process_file(const std::string& input_filename,
                  const std::string& output_filename);

std::string process_bytes(char const* input_content, size_t input_content_size);
