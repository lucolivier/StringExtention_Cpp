//
//  main.cpp
//  StringExtension
//
//  Created by Luc-Olivier on 4/24/16.
//  Copyright © 2016 Luc-Olivier. All rights reserved.
//

#include <iostream>

#include "StringExtension.hpp"

using namespace std;


int main(int argc, const char * argv[]) {
	
	string str = "Hello, World!  互";

	cout << str << " (" << str.length() << ") " << endl;
	
	String Str = "Hello, World 2!  互";
	cout << Str << " (" << Str.length() << ") " << endl;
	
	//---
	String schar = "Ç";
	
	try {
		
		cout << String::unicodeScalar("Ç") << endl;
		cout << schar.unicodeScalar() << endl;
		
	} catch (Exception &e) { e.print(); }
	
	cout << "---------------------------\n";
	cout << "#Range\n";
	
	Range r;
	cout << r << endl;
	
	try {
		//r.setStartIndex(-1);
		//Range r1 = Range(-1);
		
	} catch (Exception &e) { cout << "•" ; e.print(); }
	
	Range r2 = Range(str);
	r2.print();
	
	Range r3 = Range(10,15);
	r3.print();
	
	Range r4; // = Range(r3);
	r4.setIndices(r3);
	r4.print();
	
	
	//cout << "---------------------------\n";
	//cout << "#stringWithChar\n";
	
	//cout << String::stringWithChar("à", 20) << endl;
	
	
	cout << "---------------------------\n";
	cout << "#rangeOfStringIn & rangeOfString\n";
	
	Range nil = Range();
	
	String text = "Hello, World!";
	Range rtext = String::rangeOfStringIn(text, "World", nil);
	rtext.print();
	
	rtext = String::rangeOfStringIn(text, "!", nil);
	rtext.print();
	
	rtext = text.rangeOfString(",", nil);
	rtext.print();
	
	cout << "---------------------------\n";
	cout << "#occuranceOfStringIn & occuranceOfString\n";
	
	String text2 = "He is to watch but is he watch >to< or|     |it is to be watch to?";
	int res2 = String::occuranceOfStringIn(text2, "watch");
	cout << res2 << endl;
	
	res2 = text2.occuranceOfString("is");
	cout << res2 << endl;
	
	res2 = text2.occuranceOfString("?");
	cout << res2 << endl;
	
	cout << "---------------------------\n";
	cout << "#rangeOfStringIn & rangeOfString\n";
	
	StringExtension_ResultWithRange rwr1 = String::rangeOfStringIn(text2, "is", 1);
	rwr1.print();
	
	rwr1 = text2.rangeOfString("is", 4);
	rwr1.print();
	
	cout << "---------------------------\n";
	cout << "#removeStringIn & removeString\n";
	
	StringExtension_Result rlt = String::removeStringIn(text2, "but", 1);
	rlt.print();
	cout << text2 << endl;
	
	rlt = String::removeStringIn(text2, "is", 4);
	rlt.print();
	cout << text2 << endl;

	rlt = text2.removeString("to", 2);
	rlt.print();
	cout << text2 << endl;

	cout << "---------------------------\n";
	cout << "#removeDoubleSpaceIn & removeDoubleSpace\n";
	
	//StringExtension_Result rlt2 = String::removeDoubleSpaceIn(text2);
	StringExtension_Result rlt2 = text2.removeDoubleSpace();
	rlt2.print();
	cout << text2 << endl;
	
	cout << "---------------------------\n";
	cout << "#insertToStringIn & insertToString\n";
	
	String text3 = "The best defence is attack. Who was attacking for now?";
	StringExtension_Result rlt3 = String::insertToStringIn(text3, " I'll win because I'm ethic!", "now?", 1, StringInsertMode_After);
	rlt3.print();
	cout << text3 << endl;
	
	rlt3 = text3.insertToString(" and attack!", "attack", 1, StringInsertMode_After);
	rlt3.print();
	cout << text3 << endl;

	cout << "---------------------------\n";
	cout << "#replaceToStringIn & replaceToString\n";
	
	String text4 = "The best defence is attack, because while attacking you become cause and avoid aberration.";
	StringExtension_Result rlt4 = String::replaceToStringIn(text4, "attack", "ATTACK", 2);
	rlt4.print();
	cout << text4 << endl;
	
	rlt4 = text4.replaceToString("because", "COZ'", 1);
	rlt4.print();
	cout << text4 << endl;

	rlt4 = text4.replaceToString(".", " !!!", 1);
	rlt4.print();
	cout << text4 << endl;
	
	return 0;
}

