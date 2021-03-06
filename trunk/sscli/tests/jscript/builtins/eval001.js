//# ==++== 
//# 
//#   
//#    Copyright (c) 2002 Microsoft Corporation.  All rights reserved.
//#   
//#    The use and distribution terms for this software are contained in the file
//#    named license.txt, which can be found in the root of this distribution.
//#    By using this software in any fashion, you are agreeing to be bound by the
//#    terms of this license.
//#   
//#    You must not remove this notice, or any other, from this software.
//#   
//# 
//# ==--== 
//####################################################################################
@cc_on


import System;

var NULL_DISPATCH = null;
var apGlobalObj;
var apPlatform;
var lFailCount;


var iTestID = 52598;

function eval001() 
{
// Added to handle strict mode in JScript 7.0
@cc_on 
   @if (@_fast)  
      var k, sAct, sExp;
    
@end    

   var x=3;

   apInitTest("eval001");

   //----------------------------------------------------------------------------
   apInitScenario("1: Baseline");

   sAct = eval("for (k = 1; k != 3; k++, k*k*k) { k*k }");
   sExp = 4;
   if (sAct != sExp)
      apLogFailInfo( "Scenario 1 failed", sExp, sAct, "114");

   sAct = eval("for (k = 1; k != 3; k++, k*k*k) { k*k }k=100;");
   sExp = 100;
   if (sAct != sExp)
      apLogFailInfo( "Scenario 1 failed", sExp, sAct, "");

   sAct = eval("for (k = 1; k != 3; k++, k*k*k) { k*k }if(k=100);");
   sExp = 4;
   if (sAct != sExp)
      apLogFailInfo( "Scenario 1 failed", sExp, sAct, "");

   apInitScenario("2: eval handles function properly");
@if (!@_fast)
   eval("function foo(a) { return 2*a; }", "unsafe");
   sExp = 6;
   @if(!@_fast) sAct = foo(x);
   @else sAct = eval("foo(x)");
   @end
   if (sAct != sExp)
      apLogFailInfo( "Scenario 2 failed", sExp, sAct, "VS7 126646");
@end

  apInitScenario ("3. eval('new Date')");
      sExp = new Date();
      sAct = eval ("new Date()");
      if (sExp.getHours() != sAct.getHours())
          apLogFailInfo ("Scenario 3 failed", sExp, sAct, "VS7 #275530");


   apEndTest();
}



eval001();


if(lFailCount >= 0) System.Environment.ExitCode = lFailCount;
else System.Environment.ExitCode = 1;

function apInitTest(stTestName) {
    lFailCount = 0;

    apGlobalObj = new Object();
    apGlobalObj.apGetPlatform = function Funca() { return "Rotor" }
    apGlobalObj.LangHost = function Funcb() { return 1033;}
    apGlobalObj.apGetLangExt = function Funcc(num) { return "EN"; }

    apPlatform = apGlobalObj.apGetPlatform();
    var sVer = "1.0";  //navigator.appVersion.toUpperCase().charAt(navigator.appVersion.toUpperCase().indexOf("MSIE")+5);
    apGlobalObj.apGetHost = function Funcp() { return "Rotor " + sVer; }
    print ("apInitTest: " + stTestName);
}

function apInitScenario(stScenarioName) {print( "\tapInitScenario: " + stScenarioName);}

function apLogFailInfo(stMessage, stExpected, stActual, stBugNum) {
    lFailCount = lFailCount + 1;
    print ("***** FAILED:");
    print ("\t\t" + stMessage);
    print ("\t\tExpected: " + stExpected);
    print ("\t\tActual: " + stActual);
}

function apGetLocale(){ return 1033; }
function apWriteDebug(s) { print("dbg ---> " + s) }
function apEndTest() {}
