/** The AList is intended to provide a basic list for any class types.
*@author Marco Bertschi
*
* The AList is needed to ensure that the List behavior stays the same on
* an Arduino Board as it would be on a Computer.
*
* Usage
* ====================
* The AList class may be used as every other List class known from the .Net framework. A short example - To create a List of integers, define a variable of the type AList<int>:

      AList<int> aList;

* Elements can be added to the list by calling AList<ListItem>::Add:

      aList.Add(120);
      aList.Add(121);
      aList.Add(122);

* You can also get an element of the List by calling AList<ListItem>::GetAt. 
* A Random integer value is returned when the parameter passed to this method is greater than the value returned by AList<ListItem>::Count.

      int a = aList.GetAt(0);
      int b = aList.GetAt(1);

* **Note**: If the List is empty, this Method will return random values or throw a memory Exception. 
* It is therefore strongly recommended to get the number of elements in the list by calling AList<ListItem>::Count - If AList<ListItem>::Count returns a value below zero the list is empty (the counter is zero-based).

      int count = aList.Count();  //Return the zero based count of objects in the list

* In order to clear the List you can call

      aList.Clr();  //Clear the list

*/

#ifndef ALIST_H
#define ALIST_H

template <typename ListItem>
class AList{

private:
	/** Intended for private representation of a ListItem within the AList class - Internal use only!
	@author Marco Bertschi
	*/
	struct PrivateListItem{
		PrivateListItem* prv;
		PrivateListItem* nxt;
		ListItem crnt;
	};
	
	PrivateListItem* last;		//!< The last item of the list.
	PrivateListItem* first;		//!< The first item of the list.
	int count;					//!< Zero-based count of items within the list.

public:
	AList();
	~AList();
	ListItem First();
	ListItem Last();
	
	int Count();
	void Add(ListItem listItem);
	void RemAt(int index);
	ListItem GetAt(int index);
	void Clr();
};

#endif //ALIST_H