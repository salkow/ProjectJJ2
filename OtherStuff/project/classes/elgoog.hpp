#ifndef __ELGOOG__
#define __ELGOOG__

#include "core.hpp"
#include "./BK_T.hpp"
#include "./list_T.hpp"
#include "hashtable_T.hpp"

#include <cstring>

class Elgoog {
  public:
	struct index {};
	enum error_code {
		NO_ERROR,	  // all good, no error produced
		NULL_POINTER, // null pointer given
		EMPTY_LIST,	  // the list given is empty
		INVALID_INDEX // the index given is out of bounds (too big or <= 0)
	};

	enum match_type { EXACT, EDIT, HAMMING };

	class entry {
		struct entry_payload {
			entry_payload();
			~entry_payload();
		};
		char * word;
		struct entry_payload payload;

	  public:
		entry(char * word);
		entry(const entry &);
		~entry();
		bool operator==(entry & right) const;
		char * getWord() const; // returns the word belonging to that entry
	};

	class entry_list {
		List<entry> list;
		// int current_index; //not used

	  public:
		entry_list();
		~entry_list();
		//Η ρουτίνα αυτή προσθέτει ένα νέο entry σε μία υπάρχουσα entry_list και θα επιστρέφει
		//κατάλληλο μήνυμα τερματισμού.
		error_code add_entry(entry *);

		// H ρουτίνα αυτή επιστρέφει τον αριθμό των entries που περιέχει η δοσμένη entry_list.
		unsigned int get_number_entries() const;

		//Η ρουτίνα αυτή επιστρέφει το πρώτο entry που αντιστοιχεί στη δοσμένη entry_list.
		entry * get(int index);
	};

	class entry_index {
		match_type type;
		entry_list * list;
		BK<char> * tree;
		HashTable<entry, entry> * ht;

	  public:
		//Η ρουτίνα αυτή δημιουργεί ένα νέο ευρετήριο.Ως είσοδο δίνεται το entry_list, στα στοιχεία
		//του
		//οποίου θα χτιστεί το ευρετήριο, ο τύπος του ταιριάσμος που θα εφαρμοστεί και ένας pointer
		//που μετά το τέλος της ρουτίνας θα πρέπει να δείχνει στο ευρετήριο που δημιουργήθηκε.Τέλος
		//θα επιστρέφει κατάλληλο μήνυμα τερματισμού.
		entry_index(entry_list * el, match_type type);

		//Η ρουτίνα αυτή καταστρέφει το ευρετήριο και επιστρέφει κατάλληλο μήνυμα τερματισμού.
		~entry_index();

		// Η ρουτίνα αυτή επιστρέφει ένα σύνολο από entries, τα οποία ταιριάζουν με τη δοσμένη λέξη,
		// δεδομένου του τύπου ταιριάσματος του ευρετηρίου και του threshold που έχει δοθεί ως
		// παράμετρος.Τέλος θα επιστρέφει κατάλληλο μήνυμα τερματισμού.
		entry_list * lookup(char * word, int threshold);
	};
};

#endif
