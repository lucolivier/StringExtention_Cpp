//
//  StringExtension.hpp
//  StringExtension
//
//  Created by Luc-Olivier on 4/24/16.
//  Copyright © 2016 Luc-Olivier. All rights reserved.
//

#ifndef StringExtension_hpp
#define StringExtension_hpp

#include <stdio.h>

#endif /* StringExtension_hpp */

#include <iostream>
#include <sstream>
using namespace std;

/* ============================================================================ */

/* EXCEPTIONS */

typedef enum {
	RE_Set_IndicesLessThanZero,
	RE_Set_StartIndexNotInferiorToEndIndex,
	RE_SetRangeFromString_VoidString,
	
	SE_ReplaceString_StringToInsert_Void,
	SE_ReplaceString_StringAnchor_Void,
	SE_InsertString_StringToInsert_Void,
	SE_InsertString_StringAnchor_Void,
	SE_RemoveString_StringToRemoveVoid,
	SE_RangeOfString_VoidString,
	SE_RangeOfString_BadOccurence,
	SE_RangeOfString_RangeOutOfString,
	SE_OccurenceOfString_VoidString,
	SE_StringWithChar_NotAChar,
	SE_StringWithChar_AmountToZero,
	SE_UnicodeScalar_NotAOneCharString
} Exception_t;

const string Exception_l[] = {
	"RE_Set_IndicesLessThanZero",
	"RE_Set_StartIndexNotInferiorToEndIndex",
	"RE_SetRangeFromString_VoidString",
	
	"SE_ReplaceString_StringToInsert_Void",
	"SE_ReplaceString_StringAnchor_Void",
	"SE_InsertString_StringToInsert_Void",
	"SE_InsertString_StringAnchor_Void",
	"SE_RemoveString_StringToRemoveVoid",
	"SE_RangeOfString_VoidString",
	"SE_RangeOfString_BadOccurence",
	"SE_RangeOfString_RangeOutOfString",
	"SE_OccurenceOfString_VoidString",
	"SE_StringWithChar_NotAChar",
	"SE_StringWithChar_AmountToZero",
	"SE_UnicodeScalar_NotAOneCharString"
};

class Exception: public std::exception {
	Exception_t _type;
public:
	Exception(Exception_t type);
	virtual const char* what() const throw();
	void print();
	static Exception instance(Exception_t type);
};

/* ============================================================================ */

/* RANGE */

class Range {
	int _startIndex = -1;
	int _endIndex = 0;
	
public:
	Range();
	Range(int startIndex);

	Range(int startIndex, int endIndex);
		//throws RE_Set_StartIndexNotInferiorToEndIndex, RE_Set_IndicesLessThanZero

	//Range(Range const &range);
		// Copy constructor not needed
	
	Range(bool nothing, Range range);
		//throws RE_Set_StartIndexNotInferiorToEndIndex, RE_Set_IndicesLessThanZero
	
	Range(string string);
		//throws RE_SetRangeFromString_VoidString

	friend std::ostream &operator<<(std::ostream &output, Range &r) {
		if (r.startIndex() > -1) {
			output << r.startIndex() << "..<" << r.endIndex();
		} else {
			output << "nil";
		}
		return output;
	}
	void print();
	
	void setStartIndex(int startIndex);
		//throws RE_Set_StartIndexNotInferiorToEndIndex, RE_Set_IndicesLessThanZero
	
	int startIndex();
	
	void setStartIndexOrNil(int startIndex);
		//throws RE_Set_StartIndexNotInferiorToEndIndex
	
	void setStartIndexOrNil(size_t startIndex);
		//throws RE_Set_StartIndexNotInferiorToEndIndex
	
	void setEndIndex(int endIndex);
		//throws RE_Set_StartIndexNotInferiorToEndIndex, RE_Set_IndicesLessThanZero

	int endIndex();

	void setIndices(int startIndex, int endIndex);
		//throws RE_Set_StartIndexNotInferiorToEndIndex, RE_Set_IndicesLessThanZero

	void setIndices(Range range);
		//throws RE_Set_StartIndexNotInferiorToEndIndex, RE_Set_IndicesLessThanZero
	
	void setNil();
	bool nil();
	
	void setIndicesOfString(string string);
		//throws RE_SetRangeFromString_VoidString
	
	bool withinString(string string);
	
	int length();
	
};

/* ============================================================================ */

/* STRINGEXTENSIONRESULTS */

typedef enum {
	SER_Undefined,

	SER_ReplaceString_Success,
	SER_ReplaceString_Exception,
	
	SER_InsertString_Success,
	SER_InsertString_Exception,
	
	SER_RemoveString_Success,
	SER_RemoveString_Exception,
	
	SER_RemoveDoubleSpace_Success,
	SER_RemoveDoubleSpace_NoSpaceToRemove,
	SER_RemoveDoubleSpace_Exception,
	
	SER_RangeOfString_StringFound,
	SER_RangeOfString_StringNotFound,
	SER_RangeOfString_LastStringOccurenceFound,
	SER_RangeOfString_StringOccurenceFound,
	SER_RangeOfString_StringFoundButOccurenceNotFound,
	SER_RangeOfString_Exception,
} StringExtensionResults_t;

const string StringExtensionResults_l[] = {
	"Undefined",

	"ReplaceString_Success",
	"ReplaceString_Exception",
	
	"InsertString_Success",
	"InsertString_Exception",
	
	"RemoveString_Success",
	"RemoveString_Exception",
	
	"RemoveDoubleSpace_Success",
	"RemoveDoubleSpace_NoSpaceToRemove",
	"RemoveDoubleSpace_Exception",
	
	"RangeOfString_StringFound",
	"RangeOfString_StringNotFound",
	"RangeOfString_LastStringOccurenceFound",
	"RangeOfString_StringOccurenceFound",
	"RangeOfString_StringFoundButOccurenceNotFound",
	"RangeOfString_Exception"
};

class StringExtensionResults {
public:
	static string label(StringExtensionResults_t type);
};

class StringExtension_Result {
public:
	StringExtensionResults_t result = SER_Undefined;
	exception error;
	
	StringExtension_Result () {}
	StringExtension_Result (StringExtensionResults_t _result, exception _error);
	string resultLabel();
	string description();
	void print();
};

class StringExtension_ResultWithRange : public StringExtension_Result {
public:
	Range range;
	
	StringExtension_ResultWithRange () {}
	string description();
	void print();
};

enum StringInsertMode_t{ StringInsertMode_Before, StringInsertMode_After };

/* STRING */

class String : public string {
public:
	String() : string() {}
	String( const string &s ) : string(s) { }
	String( const char *s ) : string(s) { }
	
	void operator=(const string &s);
	void operator=(const char *s);

	StringExtension_Result replaceToString(String anchor, String string, int occurence);
	//throws SE_ReplaceString_StringToInsert_Void,
	//		 SE_ReplaceString_StringAnchor_Void,
	//		 SE_RangeOfString_VoidString,
	//		 SE_RangeOfString_BadOccurence,
	//		 SE_RangeOfString_RangeNil,
	//		 SE_RangeOfString_RangeOutOfString,
	//		 RE_SetRangeFromString_VoidString,
	//		 RE_Set_StartIndexNotInferiorToEndIndex,
	//		 RE_Set_IndicesLessThanZero
	
	static StringExtension_Result replaceToStringIn(String &target, String anchor, String string, int occurence);
	//throws SE_ReplaceString_StringToInsert_Void,
	//		 SE_ReplaceString_StringAnchor_Void,
	//		 SE_RangeOfString_VoidString,
	//		 SE_RangeOfString_BadOccurence,
	//		 SE_RangeOfString_RangeNil,
	//		 SE_RangeOfString_RangeOutOfString,
	//		 RE_SetRangeFromString_VoidString,
	//		 RE_Set_StartIndexNotInferiorToEndIndex,
	//		 RE_Set_IndicesLessThanZero
	
	StringExtension_Result insertToString(String string, String anchor, int occurence, StringInsertMode_t mode);
		//throws SE_InsertString_StringToInsert_Void,
		//		 SE_InsertString_StringAnchor_Void,
		//		 SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_BadOccurence,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	static StringExtension_Result insertToStringIn(String &target, String string, String anchor, int occurence, StringInsertMode_t mode);
		//throws SE_InsertString_StringToInsert_Void,
		//		 SE_InsertString_StringAnchor_Void,
		//		 SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_BadOccurence,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	StringExtension_Result removeString(String string, int occurence);
		//throws SE_RemoveString_StringToRemoveVoid,
		//		 SE_RangeOfString_BadOccurence,
		//		 SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	static StringExtension_Result removeStringIn(String &target, String string, int occurence);
		//throws SE_RemoveString_StringToRemoveVoid,
		//		 SE_RangeOfString_BadOccurence,
		//		 SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero

	StringExtension_Result removeDoubleSpace();
	//throws SE_RemoveString_StringToRemoveVoid,
	//		 SE_RangeOfString_BadOccurence,
	//		 SE_RangeOfString_VoidString,
	//		 SE_RangeOfString_RangeNil,
	//		 SE_RangeOfString_RangeOutOfString,
	//		 RE_SetRangeFromString_VoidString,
	//		 RE_Set_StartIndexNotInferiorToEndIndex,
	//		 RE_Set_IndicesLessThanZero
	
	static StringExtension_Result removeDoubleSpaceIn(String &target);
		//throws SE_RemoveString_StringToRemoveVoid,
		//		 SE_RangeOfString_BadOccurence,
		//		 SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	StringExtension_ResultWithRange rangeOfString(String string, int occurence);
		//throws SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_BadOccurence,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	static StringExtension_ResultWithRange rangeOfStringIn(String target, String string, int occurence);
		//throws SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_BadOccurence,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	Range rangeOfString(String string, Range range);
		//throws SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	static Range rangeOfStringIn(String target, String string, Range range);
		//throws SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	
	int occuranceOfString(String string);
		//throws SE_OccurenceOfString_VoidString,
		//		 SE_RangeOfString_VoidString,
		//		 SE_RangeOfString_RangeNil,
		//		 SE_RangeOfString_RangeOutOfString,
		//		 RE_SetRangeFromString_VoidString,
		//		 RE_Set_StartIndexNotInferiorToEndIndex,
		//		 RE_Set_IndicesLessThanZero
	
	static int occuranceOfStringIn(string target, string string);
		//throws  SE_OccurenceOfString_VoidString,
		//		  SE_RangeOfString_VoidString,
		//		  SE_RangeOfString_RangeNil,
		//		  SE_RangeOfString_RangeOutOfString,
		//		  RE_SetRangeFromString_VoidString,
		//		  RE_Set_StartIndexNotInferiorToEndIndex,
		//		  RE_Set_IndicesLessThanZero
	
	// DOES NOT WORK WITH UNICODE CHAR
	static string stringWithChar(string character, int amount);
		//throws SE_StringWithChar_AmountToZero, SEE_StringWithChar_NotAChar
	
	// DOES NOT WORK
	char32_t unicodeScalar() throw();
	static char32_t unicodeScalar(const string &str) throw();
	
};
