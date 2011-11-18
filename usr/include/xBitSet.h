#ifndef __X_BIT_SET_H__
#define __X_BIT_SET_H__

#ifndef u32
	typedef unsigned int u32;
#endif

class xBitSet
{
private:
   u32 mBits;

public:
   /// Default constructor initializes this bit set to all zeros.
   xBitSet()                         { mBits = 0; }

   /// Copy constructor.
   xBitSet(const xBitSet& in_rCopy) { mBits = in_rCopy.mBits; }

   /// Construct from an input u32.
   xBitSet(const u32 in_mask)        { mBits = in_mask; }

   /// @name Accessors
   /// @{

   /// Returns the u32 representation of the bit set.
   operator u32() const               { return mBits; }

   /// Returns the u32 representation of the bit set.
   u32 getMask() const                { return mBits; }

   /// @}

   /// @name Mutators
   ///
   /// Most of these methods take a word (ie, a BitSet32) of bits
   /// to operate with.
   /// @{

   /// Sets all the bits in the bit set to 1
   void set()                         { mBits  = 0xFFFFFFFFUL; }

   /// Sets all the bits in the bit set that are set in m.
   void set(const u32 m)              { mBits |= m; }

   /// For each bit set in s, sets or clears that bit in this, depending on whether b is true or false.
   void set(xBitSet s, bool b)       { mBits = (mBits&~(s.mBits))|(b?s.mBits:0); }

   /// Clears all the bits in the bit set to 0.
   void clear()                       { mBits  = 0; }

   /// Clears all the bits in the bit set that are set in m.
   void clear(const u32 m)            { mBits &= ~m; }

   /// Flips all the bits in the bit set that are set in m.
   void toggle(const u32 m)           { mBits ^= m; }

   /// Test if the passed bits are set.
   bool test(const u32 m) const       { return (mBits & m) != 0; }

   /// Test if the passed bits and only the passed bits are set.
   bool testStrict(const u32 m) const { return (mBits & m) == m; }

   xBitSet& operator =(const u32 m)  { mBits  = m;  return *this; }
   xBitSet& operator|=(const u32 m)  { mBits |= m; return *this; }
   xBitSet& operator&=(const u32 m)  { mBits &= m; return *this; }
   xBitSet& operator^=(const u32 m)  { mBits ^= m; return *this; }

   xBitSet operator|(const u32 m) const { return xBitSet(mBits | m); }
   xBitSet operator&(const u32 m) const { return xBitSet(mBits & m); }
   xBitSet operator^(const u32 m) const { return xBitSet(mBits ^ m); }

   /// @}
};

#ifndef xBit
	#define xBit(f) (1 << f)
#endif

__inline bool isFlagOn(xBitSet value,int flag)
{
	return value.test(1 << flag );
}
__inline bool isFlagOff(xBitSet value,int flag)
{
	return value.test(1 << flag ) ? false : true;
}

__inline void setFlag(xBitSet& value,int flag,bool condition)
{
	value.set(flag,condition);
}
__inline void enableFlag(xBitSet& value,int flag)
{
	value.set(1 << flag,true );
}
__inline void disableFlag(xBitSet& value,int flag)
{
	value.set(1 << flag,false);
}

#endif