#pragma once
#include <string>

struct Msg
{
  Msg()
    : debug(false),
      prefix(""),
      quiet(false),
      silent(false),
      hideErrors(false)
  {}

  ~Msg() {}

   void msg(std::string);
   void msg(std::string,std::string);

   void dmsg(std::string);
   void emsg(std::string);
   void imsg(std::string);
   void wmsg(std::string);

   bool debug;
   std::string prefix;

   bool quiet;  //no info msgs
   bool silent; //no warn or info msgs
   bool hideErrors;
};

