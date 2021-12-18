# ProjectJJ2

[![CMake](https://github.com/salkow/ProjectJJ2/actions/workflows/cmake.yml/badge.svg)](https://github.com/salkow/ProjectJJ2/actions/workflows/cmake.yml)

sdi1800056 sdi1800072 sdi1700142

* Έχει γίνει χρήση cmake. Για να γίνει compile το πρόγραμμα θα πρέπει να εκτελεστούν οι
  εντολές `cmake -DCMAKE_BUILD_TYPE=Release . && make`. Έτσι θα δημιουργηθούν το εκτελέσιμα
  run_tests, και inverted_google.

* Το πρόγραμμα run_tests περιέχει όλα τα test και το πρόγραμμα inverted_google είναι το inverted
  search engine που μας ζητήθηκε να φτιάξουμε. Παράδειγμα εκτέλεσης του
  inverted_google `./inverted_google test_data/small_test.txt 60`

* Έχω χρησιμοποιήσει templates για να μπορώ να φτιάξω containers με διαφορετικό τύπο δεδομένων κάθε
  φορά.

* vector class: είναι μία απομίμηση του std::vector. Έχω δημιουργήσει iterators για πιο εύκολη
  χρήση.

* string class: είναι μία απομίμηση του std::string.

* unique_ptr class: είναι μία απομίμηση του std::unique_ptr. (Ένας RAII pointer.)

* unordered_map class: ένα hash_table.

* unordered_set class: ένα hash_table που περιέχει μόνο keys.

* Στην περίπτωση που θέλω να κάνω hash έναν αριθμό χρησιμοποιώ το value του. Πιστεύω πως αυτή η λύση
  είναι γρήγορη και αρκετή. Στην περίπτωση που έχω ένα string χρησιμοποιώ έναν hash function που
  βρήκα στο internet, το οποίο είναι γρήγορο και αρκετά αποτελεσματικό.

* BK_Entry class: Είναι ενα παιδί του BK_Τ με overloaded συναρτήσεις έτσι ώστε να δουλεύει καλύτερα για τον τύπο 
  Entry. Μεγαλύτερη αλλαγή απο την μαμά κλάση είναι οτι η αναζήτηση δεν γίνεται αναδρομικά, πράγμα που την 
  επιταχύνει δραματικά.

* string_breaker function: Μια συνάρτηση που λαμβάνει ενα char* string απο λέξεις με κενά μεταξύ τους και επιστρέφει 
  ένα unordered_set με τις λέξεις του string. Έτσι γίνεται και το σπάσιμο αλλά και το deduplication μαζί.

* Exact Matching
  * Για να μπορώ να προσθέτω και να αφαιρώ εύκολα queries έχω ένα hash table με key το QueryID και
    value ένα pointer σε Query.
  * Για να μπορώ να βρίσκω εύκολα σε ποια query αντιστοιχεί μία λέξη, έχω ένα hash table με key ένα
    string και value ένα unordered_set από queries. Είναι unordered_set ώστε να έχω προσθήκη και
    διαγραφή στοιχείου σε Ο(1).

* Στη match document βρίσκουμε ποια queries αντιστοιχούν στις λέξεις του document και το προσθέτουμε
  σε ένα vector από result (ένα struct που περιέχει το document id και τα QueryIds που αντιστοιχούν
  σε αυτό).

* Στην getNext αντιγράφουμε τα query ids από το result σε ένα array και χρησιμοποιούμε την qsort για
  να ταξινομήσουμε τα αποτελέσματα.

* Edit
  * Entry
    * pair με πρώτο στοιχειό ένα string που δηλώνει τη λέξη και δεύτερο στοιχειό ένα unordered_set με query* που
      δηλώνει ποια queries έχουν τη συγκεκριμένη λέξη.
  * Για κάθε λέξη ενός query, ελέγχει αν υπάρχει ήδη μέσα στο BK tree, αν ναι βεβαιώνεται ότι η λέξη αυτή δεν είναι
    διαγεγραμμένη από το tree και προσθέτει το query στον πίνακα με τα queries που υπάρχει η λέξη.
    Αν δεν υπάρχει η λέξη στο tree δημιουργεί ένα νέο entry για τη λέξη αυτή και την προσθέτει στο tree.
  * Κατά τη διαγραφή επειδή είναι πολύ χρονοβόρο το να διαγράψουμε ένα στοιχείο από το bk tree, απλά διαγραφούμε το
    query από τον πίνακα με τα queries της λέξης και μόνο αν είναι κενός αυτός ο πίνακας αλλάζουμε μια μεταβλητή του
    στοιχειού για να δείξουμε ότι πρέπει να το αγνοούμε.
  * Κατά τη match όλες οι λέξεις που επιστρέφει η search του bk tree πρέπει κάπως να φιλτραριστούν με βάση αν περνάνε
    το tolerance που δίνεται σε κάθε query, επίσης πρέπει να ελεγχθεί αν έχουν γίνει matched κάθε λέξη του κάθε query.
    Αυτό έγινε με τη χρήση unordered_set το οποίο έχει μοναδικά δεδομένα και προσθήκη στοιχειού O(1).
  * Έχει γίνει optimization στην edit distance function έτσι ώστε να δουλεύει με maximum distance για βελτίωση
    χρονικής πολυπλοκότητας. Επίσης, έχει χρησιμοποιηθεί η bud::min function για να περιοριστούν κάποια χρονοβόρα if.

* Hamming
  * Μοναδική διαφορά από το edit ότι χρησιμοποιείται ένα array από bk trees, κάθε λέξη πάει στο συγκεκριμένο tree
    ανάλογα με το μέγεθος της.
  * Αντίστοιχα επειδή είναι πολύ απλή η hamming distance function το μόνο που χρειάστηκε ήταν να δουλεύει με maximum
    distance για να μειωθούν κάποιες μη χρήσιμες επαναλήψεις.