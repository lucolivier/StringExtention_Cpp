//
//  StringExtension.cpp
//  StringExtension
//
//  Created by Luc-Olivier on 4/24/16.
//  Copyright © 2016 Luc-Olivier. All rights reserved.
//

#include "StringExtension.hpp"
using namespace std;

/* ============================================================================
 
 ToDo: 
 
 ============================================================================ */


/* ============================================================================ */

/* EXCEPTIONS */

Exception::Exception(Exception_t type) {
	_type = type;
}
const char* Exception::what() const throw() {
	return Exception_l[_type].c_str();
}
void Exception::print() { cout << what() << endl; }

Exception Exception::instance(Exception_t type) {
	return Exception(type);
}

/* ============================================================================ */

/* RANGE */

Range::Range() { _startIndex = -1; _endIndex = 0; }

Range::Range(int startIndex) {
	try { setStartIndex(startIndex); } catch (Exception &e) { throw e; }
}

Range::Range(int startIndex, int endIndex) {
	try { setIndices(startIndex, endIndex); } catch (Exception &e) { throw e; }
}

// Copy constructor not needed
//Range::Range(Range const &range) {
//	_startIndex = range._startIndex;
//	_endIndex = range._endIndex;
//}

Range::Range(bool nothing, Range range) {
	try {
		setIndices(range.startIndex(), range.endIndex());
	} catch (Exception &e) { throw e; }
}

Range::Range(string string) {
	try { setIndicesOfString(string); } catch (Exception &e) { throw e; }
}

void Range::print() { cout << *this << endl; }

void Range::setStartIndex(int startIndex) {
	if (startIndex < 0) { throw Exception::instance(RE_Set_IndicesLessThanZero); }
	if (startIndex >= _endIndex) { throw Exception::instance(RE_Set_StartIndexNotInferiorToEndIndex); }
	_startIndex = startIndex;
}
int Range::startIndex() { return _startIndex; }

void Range::setStartIndexOrNil(int startIndex) {
	if (startIndex > _endIndex) { throw Exception::instance(RE_Set_StartIndexNotInferiorToEndIndex); }
	_startIndex = (startIndex < 0) ? -1 : startIndex;
}

void Range::setStartIndexOrNil(size_t startIndex) {
	if ((startIndex != string::npos) && (startIndex > _endIndex)) { throw Exception::instance(RE_Set_StartIndexNotInferiorToEndIndex); }
	_startIndex = (startIndex == string::npos) ? -1 : (int)startIndex;
}

void Range::setEndIndex(int endIndex) {
	if (endIndex <= _startIndex) { throw Exception::instance( RE_Set_StartIndexNotInferiorToEndIndex); }
	if (endIndex < 0) { throw Exception::instance(RE_Set_IndicesLessThanZero); }
	_endIndex = endIndex;
}
int Range::endIndex() { return _endIndex; }

void Range::setIndices(int startIndex, int endIndex) {
	if (startIndex >= endIndex) { throw Exception::instance( RE_Set_StartIndexNotInferiorToEndIndex); }
	try { setEndIndex(endIndex); setStartIndex(startIndex);
	} catch (Exception &e) { throw e; }
}

void Range::setIndices(Range range) {
	if (range.nil()) { setNil(); return; }
	try {
		setIndices(range.startIndex(), range.endIndex());
	} catch (Exception &e) { throw e; }
}

void Range::setNil() { _startIndex = -1; }
bool Range::nil() { return (_startIndex > -1) ? false : true; }

void Range::setIndicesOfString(string string) {
	if (string.length() == 0) { throw Exception::instance(RE_SetRangeFromString_VoidString); }
	_startIndex = 0;
	_endIndex = (int)string.length();
}

bool Range::withinString(string string) {
	if ((!nil()) && (_endIndex <= string.length()) ) { return true; }
	return false;
}

int Range::length() {
	if (nil()) { return -1; }
	return endIndex()-startIndex();
}

/* ============================================================================ */

/* STRINGEXTENSIONRESULTS */

string StringExtensionResults::label(StringExtensionResults_t type) {
	return StringExtensionResults_l[type];
}

StringExtension_Result::StringExtension_Result (StringExtensionResults_t _result, exception _error) {
		result = _result; error = _error;
}
string StringExtension_Result::resultLabel() {
	return StringExtensionResults::label(result);
}
string StringExtension_Result::description() {
	return "Result: "+resultLabel();
}
void StringExtension_Result::print() {
	cout << description() << endl;
}

string StringExtension_ResultWithRange::description() {
	ostringstream buf;
	buf << "Result: " << range << " (" << resultLabel() << ")";
	return buf.str();
}
void StringExtension_ResultWithRange::print() {
	cout << description() << endl;
}


/* STRING */

void String::operator=(const string &s) { *this = s; }
void String::operator=(const char *s) { *this = s; }

StringExtension_Result String::replaceToString(String anchor, String string, int occurence) {
	try { return String::replaceToStringIn(*this, anchor, string, occurence); }
	catch (Exception &e) { throw e; }
}

StringExtension_Result String::replaceToStringIn(String &target, String anchor, String string, int occurence) {
	if (string.length() == 0) { throw Exception::instance(SE_ReplaceString_StringToInsert_Void); }
	if (anchor.length() == 0) { throw Exception::instance(SE_ReplaceString_StringAnchor_Void); }
	
	StringExtension_ResultWithRange result;
	
	try {
		result  = String::rangeOfStringIn(target, anchor, occurence);
		if (result.range.nil()) {
			return StringExtension_Result(result.result, result.error);
		}
		
		target.replace(result.range.startIndex(), result.range.length(), string);

		return StringExtension_Result(SER_ReplaceString_Success, result.error);
		
	} catch (Exception &exception) { throw exception; }
}

StringExtension_Result String::insertToString(String string, String anchor, int occurence, StringInsertMode_t mode) {
	try { return String::insertToStringIn(*this, string, anchor, occurence, mode); }
	catch (Exception &e) { throw e; }
}

StringExtension_Result String::insertToStringIn(String &target, String string, String anchor, int occurence, StringInsertMode_t mode) {
	if (string.length() == 0) { throw Exception::instance(SE_InsertString_StringToInsert_Void); }
	if (anchor.length() == 0) { throw Exception::instance(SE_InsertString_StringAnchor_Void); }
	
	StringExtension_ResultWithRange result;
	
	try {
		result  = String::rangeOfStringIn(target, anchor, occurence);
		if (result.range.nil()) {
			return StringExtension_Result(result.result, result.error);
		}
		if (mode == StringInsertMode_Before) {
			target.insert(result.range.startIndex(), string);
			
		} else {
			target.insert(result.range.endIndex(), string);
		}
		return StringExtension_Result(SER_InsertString_Success, result.error);
		
	} catch (Exception &exception) { throw exception; }
}

StringExtension_Result String::removeString(String string, int occurence) {
	try { return String::removeStringIn(*this, string, occurence); }
	catch (Exception &e) { throw e; }
}

StringExtension_Result String::removeStringIn(String &target, String string, int occurence) {
	if (string.length() == 0) { throw Exception::instance(SE_RemoveString_StringToRemoveVoid); }
	
	StringExtension_ResultWithRange resultRange;
	
	try {
		
		resultRange = String::rangeOfStringIn(target, string, occurence);
		if (resultRange.range.nil()) {
			return StringExtension_Result(resultRange.result, resultRange.error);
		}
		
		target.replace(resultRange.range.startIndex(), resultRange.range.length(),"");
		return StringExtension_Result(SER_RemoveString_Success, resultRange.error);
		
	} catch (Exception &exception) { throw exception; }
}

StringExtension_Result String::removeDoubleSpace() {
	try { return String::removeDoubleSpaceIn(*this); }
	catch (Exception &e) { throw e; }
}

StringExtension_Result String::removeDoubleSpaceIn(String &target) {
	StringExtension_ResultWithRange resultRange;
	StringExtension_Result result;
	try { resultRange.range.setIndicesOfString(target); }
	catch (Exception &e) { throw e; }
	int cpt = 0;
	while (!resultRange.range.nil()) {
		
		try {
			
			resultRange.range = String::rangeOfStringIn(target,"  ",Range());
			if (!resultRange.range.nil()) {
				
				result = String::removeStringIn(target,"  ",1);
				if (result.result == SER_RemoveString_Success) {
					cpt++;
				}
			}
			
		} catch (Exception exception) { throw exception; }
	}
	if (cpt == 0) {
		result.result = SER_RemoveDoubleSpace_NoSpaceToRemove;
		return result;
	} else if (result.result ==  SER_RemoveString_Success) {
		result.result = SER_RemoveDoubleSpace_Success;
		return result;
	}
	result.result = SER_RemoveString_Exception;
	return result;
}

StringExtension_ResultWithRange String::rangeOfString(String string, int occurence) {
	try { return String::rangeOfStringIn(*this, string, occurence); }
	catch (Exception &e) { throw e; }
}

StringExtension_ResultWithRange String::rangeOfStringIn(String target, String string, int occurence) {
	// 1=1st, -1=last, n=nth
	
	if (string.length() == 0) { throw Exception::instance(SE_RangeOfString_VoidString); }
	if (occurence < -1) { throw Exception::instance(SE_RangeOfString_BadOccurence); }
	
	StringExtension_ResultWithRange result;
	
	if (occurence == 1) {
		result.range = String::rangeOfStringIn(target, string, Range());
		result.result = (!result.range.nil())
		? SER_RangeOfString_StringFound
		: SER_RangeOfString_StringNotFound;
		
	} else {
		Range currentRange;
		try { currentRange = Range(target); } catch (Exception &e) { throw e; }
		Range resultRange;
		Range resultRangeBkp;
		int cpt = 0;
		
		while (cpt != occurence) {
			
			try { resultRange = String::rangeOfStringIn(target, string, currentRange); }
			catch (Exception &e) { throw e; }
			
			if (!resultRange.nil()) {
				cpt += 1;
				if ((resultRange.endIndex()) >= target.length()) { break; }
				currentRange.setIndices(resultRange.endIndex(), (int)target.length());
			} else {
				break;
			}
			resultRangeBkp = resultRange;
		}
		if (occurence == -1) {
			if (resultRange.nil()) {
				result.range = resultRangeBkp;
			} else {
				result.range = resultRange;
			}
			result.result = (!result.range.nil())
			? SER_RangeOfString_LastStringOccurenceFound
			: SER_RangeOfString_StringNotFound;
		} else {
			if (cpt == 0) {
				result.range = resultRange;
				result.result = SER_RangeOfString_StringNotFound;
			} else if (cpt == occurence) {
				result.range = resultRange;
				result.result = SER_RangeOfString_StringOccurenceFound;
			} else {
				result.range.setNil();
				result.result = SER_RangeOfString_StringFoundButOccurenceNotFound;
			}
		}
		
	}
	return result;
}

Range String::rangeOfString(String string, Range range) {
	try { return rangeOfStringIn(*this, string, range); } catch (Exception &e) { throw e; }
}

Range String::rangeOfStringIn(String target, String string, Range range) {
	if (string.length() == 0) { throw Exception::instance(SE_RangeOfString_VoidString); }
	
	Range currentRange;
	
	if (!range.nil()) {
		if (!range.withinString(target)) { throw Exception::instance(SE_RangeOfString_RangeOutOfString); }
		try { currentRange = Range(true,range); } catch (Exception &e) { throw e; }
	} else {
		currentRange =  Range(target);
	}
	
	try {
		currentRange.setStartIndexOrNil( target.find(string, currentRange.startIndex() ) );
		//currentRange.setStartIndexOrNil( target.indexOf(string, currentRange.startIndex()) );
		if (!currentRange.nil()) {
			currentRange.setEndIndex((int)currentRange.startIndex()+(int)string.length());
		}
		return currentRange;
	} catch (Exception &e) { throw e; }
}


int String::occuranceOfString(String string) {
	try { return occuranceOfStringIn(*this, string); } catch (Exception &e) { throw e; }
}

int String::occuranceOfStringIn(string target, string string) {
	if (string.length() == 0) { throw Exception::instance(SE_OccurenceOfString_VoidString); }
	Range resultRange;
	Range currentRange;
	try { currentRange = Range(target); } catch (Exception &e) { throw e; }
	int cpt = 0;
	while (true) {
		try { resultRange = String::rangeOfStringIn(target, string, currentRange); }
		catch (Exception &e) { throw e; }
		if (!resultRange.nil()) {
			cpt += 1;
			if ((resultRange.endIndex()) >= target.length()) { break; }
			currentRange.setIndices(resultRange.endIndex(),(int)target.length());
		} else {
			break;
		}
	}
	return cpt;
}

// DOES NOT WORK WITH UNICODE CHAR
string String::stringWithChar(string character, int amount) {
	cout << character.length() << endl;
	if (character.length() != 1) { throw Exception::instance(SE_StringWithChar_NotAChar); }
	if (amount < 1) { throw Exception::instance(SE_StringWithChar_AmountToZero); }
	string res;
	for (int i=0; i<amount; i++) {
		res.append(character);
	}
	return res;
}

// DOES NOT WORK
char32_t String::unicodeScalar() throw() { return String::unicodeScalar(*this); }

char32_t String::unicodeScalar(const string &str) throw() {
	if ((str.length() > 2) || (str.length() < 1))  { throw Exception::instance(SE_UnicodeScalar_NotAOneCharString); }
	return str[0];
}
