#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "rev.h"

const char * sample = "{\"_id\":\"org.couchdb.user:user\",\"_rev\":\"1-61a1990f0a99f39de3cea3ce8d779e2e\",\"name\":\"user\",\"email\":\"email@example.com\",\"type\":\"user\",\"roles\":[],\"date\":\"2015-04-09T18:05:24.117Z\"}";


TEST_CASE( "rev is found", "[rev]" ) {
  REQUIRE(find_rev("") == std::string::npos);
  REQUIRE(find_rev("\"_rev\":\"") == 0);
  REQUIRE(find_rev(sample) == 31);
}


TEST_CASE( "rev is extracted", "[rev]" ) {
  REQUIRE( extract_rev(sample) ==
           std::string("1-61a1990f0a99f39de3cea3ce8d779e2e") );
}
