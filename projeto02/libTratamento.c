#include <wchar.h>
#include <wctype.h>
#include "libTratamento.h"

wchar_t removeAcento(wchar_t letra) {
    switch (letra) {
        case L'á':
            return L'a';
        case L'à':
            return L'a';
        case L'ã':
            return L'a';
        case L'â':
            return L'a';
        case L'é':
            return L'e';
        case L'ê':
            return L'e';
        case L'í':
            return L'i';
        case L'ó':
            return L'o';
        case L'õ':
            return L'o';
        case L'ô':
            return L'o';
        case L'ú':
            return L'u';
        case L'ç':
            return L'c';
        default:
            return letra;
    }
}

wchar_t pegaLetra(FILE *arq) {
    wchar_t letra;

    if(! (letra = fgetwc(arq)))
        return L'\0';
    letra = towlower(letra);
    letra = removeAcento(letra);

    return letra;
}
