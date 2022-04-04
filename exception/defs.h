#ifndef KAORI_EXCEPTION_
#define KAORI_EXCEPTION_

#ifdef __cplusplus
extern "C" {
#endif

extern const char* excinfo;
extern int excno;

int catche(int, void*(*)(), void*);

#ifdef __cplusplus
}
#endif

#define throw(c, ...) { excno = (c); sprintf(excinfo, __VA_ARGS__); };
#define throwr(c, ...) throw(c, __VA_ARGS__) return 

#define throws(...)
enum ExcCode_t {
    noexception,
    anyexception,
    unknown_exception,
    illegal_memory_address,
    out_of_memory,

    class_not_found,
    duplicate_class,
    // member_not_found,
    duplicate_member,
    error_type,
};

#endif