/*
Project: Printer Queue extra credit example.
By: Michael Cooper
started: 3/21/14
date last modified: 3/22/14
Compiler: Microsoft Visual Studio 2012 v110
File Type: Implimentation file for the Printer queue
*/
#ifndef ReadyQueue_cpp
#define ReadyQueue_cpp
#include "ReadyQueue.h"
#include <time.h>

using namespace std;


//constructor sets position = 1 and gives Queue it's size
ReadyQueue::ReadyQueue()
{
        srand(time(NULL)); //seed the rand function
        currentSize = 0;
	nextSlot = 1;//start at 1 in order to avoid the 0th nextSlot. Makes division possible
	
	PCB blank; //the pointers in Queue initilize to a struct that's always blank
	blank.state = NEW;
	blank.Priority = 50;//initialize to the lowest priority 
	blank.ID = -1;//initialize to -1 for checks position check

	PCB newPCB; 
	newPCB.state = NEW;
	newPCB.Priority = 50;//initialize to the lowest priority 
	newPCB.ID = -1;//initialize to -1 for checks position check

	for (int i = 0; i < qSIZE; i++)//initialize the array
	{
		Queue[i] = &blank;
	}
	

	for (int j = 0; j < tableSize; j++) 
	{
		newPCB.ID = j + 1;
		newPCB.Priority = j + 1;
		PCBTable[j] = newPCB;
	}
}


//sends in the new task to be added by its entry number, returns success or failure
bool ReadyQueue::insertProc(int ID, bool randomPriority, bool messages)
{
	PCB * toAdd = &PCBTable[(ID-1)]; //-1 to account for the 0th slot in the PCB table


	if ( (toAdd->state == NEW) || (toAdd->state == TERMINATED)) //insert only processes not in the queue
	  {

	    toAdd->state = READY;

	    if (randomPriority) toAdd->Priority = rand() % 50 + 1;

	    //add the item
	    Queue[nextSlot] = toAdd;
	    int location = nextSlot;//starting location for the new item being added
	    if (messages) cout << "inserting " << toAdd->ID<< " into " << location << endl;
	    //while the Priority is higher than that of it's parent, swap the two
	    while((toAdd->Priority < Queue[location / 2]->Priority) && (location != 1))
	      {
		if (messages) cout << "\nSwapping with the root." << endl;
		Queue[location] = Queue[location / 2];//swap the root
		Queue[location / 2] = toAdd;
		location = (location / 2);
	      }
	    nextSlot++;//incriment the next empty nextSlot
	    currentSize++;
	  
	    return true;
	  }
	  else return false;
}


//removes the front job and displays it's ID number
PCB ReadyQueue::removeHighestProc(bool messages)
{
  PCB * top = Queue[1];
  Queue[1]->state = RUNNING;
  if (messages) cout << "Removing " << top->ID << endl;
  for (int i = 1; i <= currentSize; i++)
  {
	  Queue[i]->Priority += -1;
  }
  reHeapify(messages); //pass on from client whether or not we should be sending messages.
  currentSize--;
  return *top; //dereference so that we are returning the PCB instead of the pointer
}

//re-Heapiy the array after a job has been printed.
void ReadyQueue::reHeapify(bool messages)
{
	/*
	 * Still need to fix some of the references to .ID
	 * Still need to change to pass by reference
	 * For test 2, we may need to add a function that lets us change the priorities. Because it says pick from random priorities..
	 * Let me know what you think.
	 */

	nextSlot--;//decriment nextSlot because tree is shrinking by 1
	*(Queue[1]) = *(Queue[nextSlot]);//move the lowest data member to the top. Change just the value by dereferencing.
	Queue[nextSlot]->Priority = -1;//reset Priority for the last nextSlot
	Queue[nextSlot]->state = TERMINATED;
	int loc = 1; //current location of the lase element in the array

	while(loc != nextSlot -1)//ensures that at most the item will return to the end
	{
		//insure that we don't fall off the tree
		if(((loc * 2) > nextSlot)||(Queue[loc*2]->Priority == -1))
			return;
		if ((Queue[loc]->Priority > Queue[loc * 2]->Priority) ||
			(Queue[loc]->Priority > Queue[loc * 2 + 1]->Priority))
		{
			PCB * PCBHolder = Queue[loc];//pull the current message
			//See which one has a higher Priority and swap
			if (Queue[loc * 2]->Priority < Queue[(loc * 2) + 1]->Priority || Queue[(loc * 2)+1]->Priority == -1)
			{
				if (Queue[loc * 2]->Priority == -1 || Queue[loc * 2]->Priority > Queue[loc]->Priority)
					return;
				
				if (messages) cout << "\nSwapping " << Queue[loc]->Priority
					<< " with the left child" << endl;

				Queue[loc] = Queue[loc*2];
				Queue[loc*2] = PCBHolder;
				loc = (loc * 2);
				continue;//check the loop again
			}
			else
			{
				if(Queue[(loc*2)+1]->Priority == -1)
				  return;

				if (messages) cout << "\nSwapping " << Queue[loc]->Priority
					<< " with the right child" << endl;
				
				Queue[loc] = Queue[loc*2+1];
				Queue[loc*2+1] = PCBHolder;
				loc = (loc * 2+1);
				continue;//check the loop again
			}

		}
		else
			return;//exit if at correct Priority
	}
}

int ReadyQueue::size()
{
  return currentSize;
}

//displays everythign left in the queue.
void ReadyQueue::displayQueue()
{
	cout << "\nThe queue contains: [";

	for(int i = 1; i < nextSlot; i++)
	{
	  if (i != (nextSlot - 1 )) cout << Queue[i]->ID << " p=" << Queue[i]->Priority << ", ";
	  else  cout << Queue[i]->ID << " p=" << Queue[i]->Priority << "] ";
	}

	if (currentSize == 0) cout << "]";

	cout << endl;
}


#endif
