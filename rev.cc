#include <string>

namespace {
  
  const char * lead_in = "\"_rev\":\"";

};

size_t find_rev(const std::string& s) {

  return s.find(lead_in);
}

const std::string extract_rev(const std::string& s) {

  size_t offset = find_rev(s);
  if (offset != std::string::npos) {

    offset += sizeof(lead_in);
      size_t end = s.find('"', offset);

      return s.substr(offset, end-offset);
    // curl_easy_setopt(curl, CURLOPT_URL, "");
    // "http://127.0.0.1:15986/-/user/org.couchdb.user:user/-rev/1-c12404fd76fc533601e72d41c116336a"
    // 
    }

  return "";
}
