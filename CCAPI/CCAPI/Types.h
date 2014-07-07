#ifndef __CCAPITYPES__
#define __CCAPITYPES__

using namespace std;
#include <memory>
#include <list>
#include <iostream>
#include <vector>
#include <memory>

class Variant
{
public:
	Variant(float v);
	Variant(short v);
	Variant(char v);
	Variant(long v);
	Variant(long long v);
	Variant() { pt_obj.cPt = (char*)&value;}


	float asFloat() { return *pt_obj.fPt; }
	long asLong() { return *pt_obj.lPt; }
	short asShort() { return *pt_obj.sPt; }
	char asChar() { return *pt_obj.cPt; }
	long long asLongLong() { return *pt_obj.llPt; }
	char * asPointer() { return pt_obj.cPt; }

	short *convertToShortPointer(char *pt) { pt_obj.cPt = pt; return pt_obj.sPt; }
	long *convertToLongPointer(char *pt) { pt_obj.cPt = pt; return pt_obj.lPt; }
	long long *convertToLongLongPointer(char *pt) { pt_obj.cPt = pt; return pt_obj.llPt; }
	float *convertToFloatPointer(char *pt) { pt_obj.cPt = pt; return pt_obj.fPt; }

	short convertToShort(char *pt) { pt_obj.cPt = pt; return *pt_obj.sPt; }
	long convertToLong(char *pt) { pt_obj.cPt = pt; return *pt_obj.lPt; }
	long long convertToLongLong(char *pt) { pt_obj.cPt = pt; return *pt_obj.llPt; }
	float convertToFloat(char *pt) { pt_obj.cPt = pt; return *pt_obj.fPt; }

private:
	union pointer
	{
		char *cPt;
		short *sPt;
		long *lPt;
		long long *llPt;
		float *fPt;
	} pt_obj;
	char value[8];
};

struct AddressOffset  //this is a pointer helper that contains an address and an offset
{
	unsigned long address;
	unsigned int offset;
	AddressOffset(unsigned long a, unsigned int o) { address = a; offset = o; }
};

typedef list<unsigned int> PointerOffsets; //this is a list of offsets for a pointer
typedef list<AddressOffset> AddressOffsets; //this is a list of address and offset items


class PointerObj
{
public:
	PointerObj(AddressOffsets b);
	PointerObj(const unsigned long address, const PointerOffsets &pt);

	AddressOffsets pointers;
	unsigned long resolved; //this is our resolved address

	unsigned int updateCount;
	void fromPointerOffsets(const unsigned long address, const PointerOffsets &pt);
	bool equal(PointerObj p);
	void update();
	PointerOffsets getOffsets();
};
typedef shared_ptr<PointerObj> PointerItem;

/* An AddressItem is either an address that we are focusing on or a base of a pointer that we are focusing on.
   The value is the memory at that address or whatever the resolved pointer address is. */
class AddressObj
{
public:
	unsigned long address; //this is our base address
	long long value; //this is the memory from either our base address or our resolved address
	char sign; //signed or unsigned
	char type; //1-4byte, float
	PointerItem pointer; //our pointer info if we are using one
	AddressObj();
	AddressObj(unsigned long a, unsigned long v, char t, char s); //a single address
	AddressObj(unsigned long a, unsigned long v, char s); //a single address
	AddressObj(unsigned long a, PointerOffsets p, char t, char s); //a pointer
	void init(unsigned long a, unsigned long v, char t, char s);
	AddressObj &operator=(AddressObj t);	
	bool isPointer() { return pointer != nullptr; }
};
typedef shared_ptr<AddressObj> AddressItem;

typedef vector<AddressItem> AddressList;
#endif
