Vitória Marca Santa Lucia

Segunda entrega: Agenda pBuffer.

O desafio proposto é de desenvolver uma espécie de agenda que armazena informações como nome, idade e e-mail de uma pessoa. O programa deve ter a capacidade de adicionar, remover, buscar (e mostrar informações de) uma pessoa pelo seu nome e listar todos os registros. A grande questão é implementar todas essas funcionalidades sem declarar variáveis, structs etc., mas apenas ponteiros.

A princípio, tudo está se comportando da maneira que deve ser. Tive alguns desafios na implementação, como a tentativa de colocar os primeiros 4 bytes reservados do "pBuffer" com uma "tag", um nome, que na verdade é o contador de quantas pessoas têm registradas. Em uma das funções, essa atribuição não foi possível e gerava muitos erros, mas em outras, sim.
Também tive bastante dificuldade em entender o problema, precisei pesquisar um pouco para adquirir repertório e poder resolver o que estava sendo proposto. A priori, eu não tinha a menor ideia de como implementar apenas com ponteiros. O que me ajudou muito foi reler as instruções da tarefa, "fazer o trabalho do sistema operacional", então lembrei que a memória é sequencial, que eu poderia ter espaços fixos destinados a cada registro, e a partir dai o trabalho foi fazer código.

Testei casos como:
Pedir para buscar e listar com agenda vazia, fazer a mesma coisa com nomes que não estavam inseridos (as flags são diferentes), fazer remoções, adições, etc.
