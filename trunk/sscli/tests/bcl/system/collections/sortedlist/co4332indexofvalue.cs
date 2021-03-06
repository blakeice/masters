// ==++==
//
//   
//    Copyright (c) 2002 Microsoft Corporation.  All rights reserved.
//   
//    The use and distribution terms for this software are contained in the file
//    named license.txt, which can be found in the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by the
//    terms of this license.
//   
//    You must not remove this notice, or any other, from this software.
//   
//
// ==--==
using System.IO;
using System.Text;
using System;
using System.Collections;
public class Co4332IndexOfValue
    :
    IComparer
{
    internal static String strName = "SortedList.IndexOfValue";
    internal static String strTest = "Co4332IndexOfValue";
    internal static String strPath = "";
    public virtual int Compare  
        (
        Object p_obj1
        ,Object p_obj2
        )
    {
        return String.Compare( p_obj1.ToString() ,p_obj2.ToString() );
    }
    public virtual bool runTest()
    {
        Console.Error.WriteLine( "Co4332IndexOfValue  runTest() started." );
        String strLoc="Loc_000oo";
        StringBuilder sblMsg = new StringBuilder( 99 );
        int iCountErrors = 0;
        int iCountTestcases = 0;
        SortedList sl2 = null;
        StringBuilder sbl3 = new StringBuilder( 99 );
        StringBuilder sbl4 = new StringBuilder( 99 );
        StringBuilder sblWork1 = new StringBuilder( 99 );
        String str5 = null;
        String str6 = null;
        String str7 = null;
        String s1 = null;
        String s2 = null;
        String s3 = null;
        String s4 = null;
        int[] in4a = new int[9];
        int nCapacity = 100;
        bool bol = false;
        int i = 0;
        int j = 0;
        try
        {
        LABEL_860_GENERAL:
            do
            {
                strLoc="100cc";
                iCountTestcases++;
                sl2 = new SortedList( this );
                iCountTestcases++;
                if ( sl2 == null )
                {
                    Console.WriteLine( strTest+ "E_101" );
                    Console.WriteLine( strTest+ "SortedList creation failure" );
                    ++iCountErrors;
                    break;
                }
                iCountTestcases++;
                if ( sl2.Count  != 0 )
                {
                    Console.WriteLine( strTest+ "E_102" );
                    Console.WriteLine( strTest+ "New SortedList is not empty" );
                    ++iCountErrors;
                }
                iCountTestcases++;
                try 
                {
                    j = sl2.IndexOfValue((String)null);
                    if (j != -1) 
                    {
                        ++iCountErrors;
                        Console.Error.WriteLine(  "POINTTOBREAK: Error Err_39re! - should return -1..."  );
                        Console.Error.WriteLine( "returned: " + j );
                    }
                }
                catch (Exception exc) 
                {
                    ++iCountErrors;
                    Console.Error.WriteLine(  "POINTTOBREAK: Error Err_59f! - " + exc.ToString()  );
                }
                iCountTestcases++;
                j = sl2.IndexOfValue("No_Such_Val");
                if (j != -1) 
                {
                    ++iCountErrors;
                    Console.Error.WriteLine(  "POINTTOBREAK: Error Err_658d! - should return -1..."  );
                    Console.Error.WriteLine( "returned: " + j );
                }
                strLoc="100.5cc";
                sl2.Add ("Key_0", null);
                j = sl2.IndexOfValue(null);
                if (j == -1) 
                {
                    ++iCountErrors;
                    Console.Error.WriteLine(  "POINTTOBREAK: Error Err_47d! - should not return -1..."  );
                }
                strLoc="100.6cc";
                sl2.Add ("Key_1", "Val_Same");
                sl2.Add ("Key_2", "Val_Same");
                try 
                {
                    j = sl2.IndexOfValue("Val_Same");
                    if (j != 1) 
                    {
                        ++iCountErrors;
                        Console.Error.WriteLine(  "POINTTOBREAK: Error Err_985g! - IndexOf failed to return first occurrence..."  );
                        Console.Error.WriteLine( "returned: " + j );
                    }
                }
                catch (Exception exc) 
                { 
                    ++iCountErrors;
                    Console.Error.WriteLine(  "POINTTOBREAK: Error Err_095! - " + exc.ToString() );
                }
                strLoc="100.7cc";
                sl2.Clear();
                strLoc="Loc_141aa";
                for (i=0; i<100; i++) 
                {
                    ++iCountTestcases;
                    sblMsg.Length =  0 ;
                    sblMsg.Append("key_");
                    sblMsg.Append(i);
                    s1 = sblMsg.ToString();
                    sblMsg.Length =  0 ;
                    sblMsg.Append("val_");
                    sblMsg.Append(i);
                    s2 = sblMsg.ToString();
                    sl2.Add (s1, s2);
                }
                strLoc="Loc_151aa";
                for (i=0; i < sl2.Count ; i++) 
                {
                    sblMsg.Length =  0 ;
                    sblMsg.Append("key_"); 
                    sblMsg.Append(i);
                    s1 = sblMsg.ToString();
                    sblMsg.Length =  0 ;
                    sblMsg.Append("val_"); 
                    sblMsg.Append(i);
                    s2 = sblMsg.ToString();
                    ++iCountTestcases;
                    s3 = (String) sl2.GetByIndex(sl2.IndexOfKey(s1)) ; 
                    s4 = (String) sl2.GetByIndex(sl2.IndexOfValue(s2)) ; 
                    strLoc="Loc_161aa";
                    if (!s3.Equals (s4)) 
                    { 
                        ++iCountErrors;
                        sblMsg.Length =  0 ;
                        sblMsg.Append( "POINTTOBREAK: Error Err_1032bc! - s4 == " );
                        sblMsg.Append( s4 );
                        sblMsg.Append( "; s3 == " );
                        sblMsg.Append( s3 );
                        Console.Error.WriteLine(  sblMsg.ToString()  );
                    }
                    strLoc="Loc_171aa";
                    ++iCountTestcases;
                    s3 = (String) sl2.GetByIndex(sl2.IndexOfKey(s1)) ;
                    if (!s3.Equals (s2)) 
                    {
                        ++iCountErrors;
                        sblMsg.Length =  0 ;
                        sblMsg.Append( "POINTTOBREAK: Error Err_1032.2bc! - s2 == " );
                        sblMsg.Append( s2 );
                        sblMsg.Append( "; s3 == " );
                        sblMsg.Append( s3 );
                        Console.Error.WriteLine(  sblMsg.ToString()  );
                    }
                }
                ++iCountTestcases;
                sblMsg.Length =  0 ;
                sblMsg.Append("key_50");
                s1 = sblMsg.ToString();
                sl2.Remove (s1); 
                strLoc="Loc_181aa";
                ++iCountTestcases;
                j = sl2.IndexOfValue(s1);
                if (j != -1) 
                {
                    ++iCountErrors;
                    Console.Error.WriteLine(  "POINTTOBREAK: Error Err_39fd! - should return -1..."  );
                    Console.Error.WriteLine( "returned: " + j );
                }
            } while ( false );
        }
        catch( Exception exc_general )
        {
            ++iCountErrors;
            Console.Error.WriteLine(  "POINTTOBREAK: Error Err_343un! (Co4332IndexOfValue) exc_general==" + exc_general  );
            Console.Error.WriteLine(  "EXTENDEDINFO: (Err_343un) strLoc==" + strLoc  );
        }
        if ( iCountErrors == 0 )
        {
            Console.Error.WriteLine( "paSs.   SortedList\\Co4332IndexOfValue.cs   iCountTestcases==" + iCountTestcases );
            return true;
        }
        else
        {
            Console.Error.WriteLine( "FAiL!   SortedList\\Co4332IndexOfValue.cs   iCountErrors==" + iCountErrors );
            return false;
        }
    }
    public static void Main( String[] args )
    {
        bool bResult = false; 
        StringBuilder sblMsg = new StringBuilder( 99 );
        Co4332IndexOfValue cbA = new Co4332IndexOfValue();
        try
        {
            bResult = cbA.runTest();
        }
        catch ( Exception exc_main )
        {
            bResult = false;
            Console.Error.WriteLine(  "POINTTOBREAK:  FAiL!  Error Err_999zzz! (Co4332IndexOfValue) Uncaught Exception caught in main(), exc_main==" + exc_main  );
        }
        if ( ! bResult )
            Console.Error.WriteLine(  "Co4332IndexOfValue.cs   FAiL!"  );
        if ( bResult == true ) Environment.ExitCode = 0; else Environment.ExitCode = 1; 
    }
}
