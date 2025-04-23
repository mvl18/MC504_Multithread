#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define NUM_OF_REINDEERS 9
#define QUANT_ELFOS 10 // Quantidade de elfos no programa

#define CHANCE_PROBLEMA                                                        \
  5 // Chance de o elfo possuir um problema.
     // Exemplo: 10 significa que há uma chance
     // de 1 em 10 de um elfo possuir um problema,
     // 20 significa que há uma chance de 1 em 20, etc.
#define SEC_TO_US 1000000

#define USLEEP_ELFO_TRABALHANDO (rand() % (3*SEC_TO_US))
#define USLEEP_SANTA_AJUDA (2 * SEC_TO_US)
#define USLEEP_RENA_FERIAS ((rand() % 10 + 5) * SEC_TO_US)
#define USLEEP_PREPARACAO_TRENO (rand() %  (5*SEC_TO_US))
#define USLEEP_DELAY_FIM (5 * SEC_TO_US)

// #define USLEEP_ELFO_TRABALHANDO (rand() % 2000)
// #define USLEEP_SANTA_AJUDA (3000)
// #define USLEEP_RENA_FERIAS ((rand() % 10 + 5) * SEC_TO_US)
// #define USLEEP_PREPARACAO_TRENO (rand() % (5*SEC_TO_US))
// #define USLEEP_DELAY_FIM (5 * SEC_TO_US)
#endif // !__GLOBALS_H__
