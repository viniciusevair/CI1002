#include <wchar.h>
#include <wctype.h>

#include "libTratamento.h"

/*
 * Remove os acentos das vogais comumente usadas na lingua portuguesa.
 * Nao trata o casos de demais caracteres. 
 */
wchar_t removeAcento(wchar_t chave) {
    switch (chave) {
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
            return chave;
    }
}

wchar_t pegaCaractere(FILE *arq) {
    wchar_t chave;

    if(! (chave = fgetwc(arq)))
        return L'\0';
    chave = towlower(chave);
    chave = removeAcento(chave);

    return chave;
}
