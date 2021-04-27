
#ifndef ELKDB_SYMBOL_TABLE_H
#define ELKDB_SYMBOL_TABLE_H

#include <unordered_map>

/**
 * The enumeration of the identifier data type.
 */
enum IdentifierType {
    STRING,
    INTEGER,
    REAL
};

/**
 * The enumeration of the identifier class i.e; table or column.
 */
enum IdentifierFor {
    TABLE,
    COLUMN
};


/**
 * The strcuture for the single symbol table entry.
 */
struct SymbolTableEntry {
    IdentifierType type;
    IdentifierFor ifor;
};


/**
 * The type of the identifier in the symbol table.
 */
typedef const char* ConstString;


/**
 * This class is responsible for managing the symbol table operations like
 *     1. Inserting entry into the symbol table.
 *     2. Fetching the symbol table entry using the identifier.
 *     3. Getting specific value of each symbol table.
 *     etc.
 */
class SymbolTable {
    private:
        std::unordered_map<ConstString, SymbolTableEntry> symbol_map;

    public:
        SymbolTable() {}

        void insertSymbol(ConstString identifier, SymbolTableEntry entry);
        SymbolTableEntry getSymbolTableEntry(ConstString identifier);
        IdentifierType getIdentifierType(ConstString identifier);
        IdentifierFor getIdentifierInfo(ConstString identifier);
        void print();
};

/**
 * Inserts a new entry to the symbol table with the identifier as the key.
 *
 * @param identifier The identifier for the symbol table entry.
 * @param entry The symbol table entry struct.
 * @return none
 */
void SymbolTable::insertSymbol(ConstString identifier, SymbolTableEntry entry) {
    symbol_map.insert(std::pair<ConstString, SymbolTableEntry>(identifier, entry));
}

/**
 * Returns the type of the symbol table variable.
 *
 * @param identifier The identifer for the symbol table.
 * @return Returns the type of the identifier symbol.
 */
IdentifierType SymbolTable::getIdentifierType(ConstString identifier) {
    return getSymbolTableEntry(identifier).type;
}

/**
 * Returns the type of the symbol table variable like whether it a symbol
 * table for the table or column.
 *
 * @param identifier The identifier for the symbol table.
 * @return Returns the information of the identifier symbol.
 */
IdentifierFor SymbolTable::getIdentifierInfo(ConstString identifier) {
    return getSymbolTableEntry(identifier).ifor;
}

/**
 * Returns the symbol table entry object itself.
 *
 * @param identifier
 * @return The SymbolTableEntry object itself.
 */
SymbolTableEntry SymbolTable::getSymbolTableEntry(ConstString identifier) {
    return symbol_map[identifier];
}

/**
 * Prints the complete symbol table. Use this only for debugging to view
 * all the entries in the symbol table.
 *
 * @return void
 */
void SymbolTable::print() {

    unsigned n = symbol_map.bucket_count();

    std::cout << "Symbol Table has " << n << " buckets.\n";

    for (unsigned i = 0; i < n; ++i) {
        std::cout << "bucket #" << i << " contains: ";

        for (auto it = symbol_map.begin(i); it!=symbol_map.end(i); ++it)
            std::cout << "[" << it->first << ":" << &it->second << "] ";

        std::cout << "\n";
    }
}

#endif