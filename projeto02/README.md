Para este projeto fiz uso de um total de sete módulos:  
    - Beale  
    - libAVL  
    - libSubAVL  
    - libCodifica  
    - libDecodifica  
    - libCifra  
    - libTratamento  

Onde Beale é responsável pela função main, gerenciamento de entradas e
tratamento de erros. Como código principal, ele é o responsável por juntar todas
as bibliotecas (com excessão das duas bibliotecas auxiliares internas);

As libAVL e libSubAVL funcionam juntas. Tratam-se de duas bibliotecas que cuidam
de toda a estrutura de dados que guarda as cifras, sendo uma responsável pelas
chaves da cifra, guardadas no formato wchar_t, e a outra pelas posições de cada
palavra do livro cifra, guardadas no formato int. Como o nome sugere, ambas
implementam o código dos algoritmos da estrutura de dados AVL, ou seja, o
projeto utiliza uma árvore binária de árvores binárias para o armazenamento de
dados.
A uso de árvores binárias AVL para este projeto foi uma escolha que se deu por
dois motivos: Primeiramente, para promover o diálogo entre as disciplinas de
Programação 2 e Algoritmos e Estrutura de Dados 3; 
Em segundo lugar, por conta da eficiência do uso de estruturas de árvores.
Apesar da inserção um pouco mais custosa que a de listas ligadas, o programa da
cifra de Beale realiza muitas buscas, que são muito mais rápidas em arvores
binárias. Em especial, na AVL, O(log n) para a busca, em comparação com O(n) da
lista ligada. Além disso, pela natureza da árvore, não é necessário nenhum
algoritmo de ordenação para as chaves, basta realizar a leitura EmOrdem. 

As libCodifica e libDecodifica são bibliotecas de poucas funções, que fazem
justamente o que o nome sugere. Ambas as bibliotecas utilizam arquivos de texto
para entrada e saida de dados. 

Já a libCifra é responsável pela criação do arquivo de chaves em caso de
codificação ou decodificação com livro cifra, e pela leitura do arquivo de texto
em caso de decodificação com arquivo de chaves. Quando o usuário decide
codificar uma mensagem, ela também disponibiliza a opção de gravar um arquivo
contendo as chaves utilizadas.

Por fim, a libTratamento é uma biblioteca interna às libCodifica e libCifra, com
a simples função de padronizar os caracteres com letras para minúsculas e sem
acentos. Apesar de padronizar para a saida, ela utiliza wchar_t para guardar os
caracteres, ou seja, faz com que o programa beale seja capaz de trabalhar não
apenas com arquivos em ASCII mas também arquivos em UTF-8.

Todas as leituras e escritas do programa em arquivos foram feitas caractere por
caractere.
