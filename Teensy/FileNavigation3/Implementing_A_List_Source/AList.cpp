#include "AList.h"

//! Instantiates a new instance of an AList.
/*!
\return		AList<ListItem>		A new instance of an AList.
*/
template <typename ListItem>
AList<ListItem>::AList(){
	count = 0;
}
//! Destroys the instance of AList<ListItem>.
/*!
The AList<ListItem>::Clr() is called in order to free memory which
was previously occupied by the dynamically allocated list items.
\sa Clr();
*/
template <typename ListItem>
AList<ListItem>::~AList(){
	if (count > 0){
		Clr(); //Clear the List in order to free memory
	}
}
//! Adds an Item of the type ListItem to the AList.
/*!
\param		li		[ListItem]		The ListItem which is added to the AList.
\return		void	[void]		
*/
template <typename ListItem>
void AList<ListItem>::Add(ListItem li){
	PrivateListItem* pLItem = new PrivateListItem;
	pLItem->crnt = li;

	if (count > 0){
		pLItem->nxt = first;
		pLItem->prv = last;
		last->nxt = pLItem;
		last = pLItem;
		count++;
	}
	else if (count == 0){
		first = pLItem;
		first->nxt = pLItem;
		first->prv = pLItem;
		last = pLItem;
		last->nxt = pLItem;
		last->prv = pLItem;
		count++;
	}
}
//! Removes a ListItem from a given index position in the AList.
/*!
In case that there is no ListItem stored at the given index of the List 
no operation will be done and the list remains unchanged.

\param		index	[int]	The Index at which the ListItem gets removed.
\return		void	[void]
*/
template <typename ListItem>
void AList<ListItem>::RemAt(int index){
	if (index < count){
		PrivateListItem* pLItem = last;
		for (int i = 0; i <= index; i++){
			pLItem = pLItem->nxt;
		}
		pLItem->prv->nxt = pLItem->nxt;
		pLItem->nxt->prv = pLItem->prv;
		delete pLItem;
		count--;
	}
}
//! Gets a ListItem from a given index position in the AList.
/*!
In case that there is no ListItem stored at the given index of the List
this method will return a random value, or may lead to a Memory read exception.
This also applies if no item at all is stored in the list.

\param		index		[int]		The Index at which the ListItem gets removed.
\return		ListItem	[ListItem]	The ListItem at the position `index` in the list.
\sa Count()
*/
template <typename ListItem>
ListItem AList<ListItem>::GetAt(int index){
	PrivateListItem* pLItem = first;
	if (index <= count && index > 0){
		int i = 0;
		while(i < index){
			pLItem = pLItem->nxt;
			i++;
		}
		return pLItem->crnt;
	}
	return NULL;
}
//! Gets the first ListItem which is stored in the list.
/*!
A random value will be returned if no items are stored in the list.

\return		ListItem	[ListItem]    The first ListItem in the list.
\sa Last(), Count()
*/
template <typename ListItem>
ListItem AList<ListItem>::First(){
	return first->crnt;
}
//! Gets the last ListItem which is stored in the list.
/*!
A random value will be returned if no items are stored in the list.
If there is only one Item stored in the list this method returns the same value as AList<ListItem>::First().

\return		ListItem    [ListItem]	The first ListItem in the list.
\sa First(), Count()
*/
template <typename ListItem>
ListItem AList<ListItem>::Last(){
	return last->crnt;
}
//! Gets the number of ListItems in the List.
/*!
The number is zero-based - A return value `0` means that there is one item stored in the list.
Please remember that a return value of `0` means that there are no items stored in the list.

\return		int		[int]		Zero-based number of Items in the List.
*/
template <typename ListItem>
int AList<ListItem>::Count(){
	return count;
}

//! Clears the content of the List.
/*!

\return		void	[void]
*/
template <typename ListItem>
void AList<ListItem>::Clr(){
	PrivateListItem* pLItem = first;
	while(count > 0){
		PrivateListItem* tbdListItem = pLItem;
		pLItem = pLItem->nxt;
		delete tbdListItem;
		count--;
	}
}
