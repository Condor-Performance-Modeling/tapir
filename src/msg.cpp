#include "msg.h"
#include <iostream>
#include <string>
using namespace std;
// -----------------------------------------------------------------
void Msg::msg(string p,string s) { cout<<p<<":"<<prefix<<": "<<s<<endl; }
// -----------------------------------------------------------------
void Msg:: msg(string s) { cout<<s<<endl; }
void Msg::dmsg(string s) { if(debug)             msg("-D",s); }
void Msg::emsg(string s) { if(!hideErrors)       msg("-E",s); }
void Msg::imsg(string s) { if(!silent && !quiet) msg("-I",s); }
void Msg::wmsg(string s) { if(!silent)           msg("-W",s); }

