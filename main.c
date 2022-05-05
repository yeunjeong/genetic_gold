#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define gold_num 6
#define population_size 16
#define select_num 4
#define fit_num 17
#define fit_num_min 15

typedef struct Gold{
  int weight[gold_num];
  int price[gold_num];  
}Gold;

typedef struct Object{
  Gold* gold;
  int gold_exist[gold_num];
  int fitness;
}Object;

void print_population(Object* o);
int is_weight_under10(Object* o); // 안넘으면 1 반환 아니면 0
void create_population(Object* o, Gold g);
void cal_fitness(Object* o);
int is_in_selection(int selection[], int n); // 있으면 1 아니면 0
void select_population(Object* o, int selection[]); //가장 높은 4개 선택

void create_parent(Object* o, int selection[], Object* po);
void create_crossover(Object* o, Object* po);

int evaluate_min_fit(Object* o);
int evaluate_max_fit(Object* o);
int evaluate_max_fit2(Object* o);

void mutation(Object* o);

int main(void) {
  int T = 0;
  Gold gold;
  
  gold.weight[0] = 4; gold.price[0] = 6;
  gold.weight[1] = 4; gold.price[1] = 7;
  gold.weight[2] = 3; gold.price[2] = 5;
  gold.weight[3] = 2; gold.price[3] = 4;
  gold.weight[4] = 1; gold.price[4] = 3;
  gold.weight[5] = 6; gold.price[5] = 9;
  
  Object population[population_size];
  create_population(population, gold);
  cal_fitness(population);

  print_population(population);
  
  Object parent[select_num];
  int selection[select_num];
  int max_fit = evaluate_max_fit(population);
  int min_fit = evaluate_min_fit(population);
  
  while(max_fit < fit_num || min_fit < fit_num_min){
    select_population(population, selection);
    create_parent(population, selection, parent);
    create_crossover(population, parent);
    //mutation(population);
    
    cal_fitness(population);  
    max_fit = evaluate_max_fit(population);
    min_fit = evaluate_min_fit(population);
    print_population(population);
  }
  
  /*
  int min_fit = evaluate_min_fit(population);
  int max_fit = evaluate_max_fit(population);

  while(min_fit < fit_num){
    select_population(population, selection);
    create_parent(population, selection, parent);
    create_crossover(population, parent);

    while(max_fit < fit_num){
      mutation(population); 
      cal_fitness(population);
      max_fit = evaluate_max_fit(population);
    }
    
    cal_fitness(population);
    min_fit = evaluate_min_fit(population);
    
    print_population(population);
  }
      */
  
  int max = evaluate_max_fit2(population);
  printf("답 : [ ");
  for(int i = 0; i < gold_num; i++){
    printf("%d ", population[max].gold_exist[i]);
  }
  printf("] fit : %d\n", population[max].fitness);
  printf("finish");
  return 0;
}

int is_weight_under10(Object* o){
  int weight = 0;
  for(int i = 0; i < gold_num; i++){
    if(o->gold_exist[i] == 1){
      weight += o->gold->weight[i];
    }
  }
  if(weight <= 10) return 1;
  else return 0;
}

void create_population(Object* o, Gold g){
  srand(time(NULL));
  for(int i = 0; i < population_size; i++){
    o[i].gold = &g;
    for(int j = 0; j < gold_num; j++){
      o[i].gold_exist[j] = rand()%2;
    }
    if(!is_weight_under10(&o[i])) i--;
  }
}

void cal_fitness(Object* o){
  for(int i = 0; i < population_size; i++){
    o[i].fitness = 0;
    for(int j = 0; j < gold_num; j++){
      if(o[i].gold_exist[j] == 1){
        o[i].fitness += o[i].gold->price[j];
      }
    }
  }
}

int is_in_selection(int selection[], int n){
  for(int i = 0; i < select_num; i++){
    if(n == selection[i]) return 1;
  }
  return 0;
}

void select_population(Object* o, int selection[]){
  int max = 0;
  int max_index = 0;
  for(int i = 0; i < select_num; i++){
    for(int j = 0; j < population_size; j++){
      if(!is_in_selection(selection, j) && max < o[j].fitness){
        max = o[j].fitness;
        max_index = j;
      }
    }
    selection[i] = max_index;
    max = 0;
  }
}
/*
void select_population(Object* o, int selection[]){
  int max = 0;
  int max_index = 0;
  int max_select = evaluate_max_fit(o)+1;
  for(int i = 0; i < select_num; i++){
    for(int j = 0; j < population_size; j++){
      if(!is_in_selection(selection, j) && max_select > o[j].fitness && max < o[j].fitness){
        max = o[j].fitness;
        max_index = j;
      }
    }
    selection[i] = max_index;
    max_select = o[max_index].fitness;
    max = 0;
  }
}
*/

void create_parent(Object* o, int selection[], Object* po){
  for(int i = 0; i < select_num; i++){
    po[i].fitness = o[selection[i]].fitness;
    po[i].gold = o[0].gold;
    for(int j = 0; j < gold_num; j++){
      po[i].gold_exist[j] = o[selection[i]].gold_exist[j];
    }
  }
}

void create_crossover(Object* o, Object* po){
  int i = 0;
  int cross_index = 1;
  while(i < population_size){
    for(int j = 0; j < cross_index; j++){
      o[i].gold_exist[j] = po[i/4].gold_exist[j];
    }
    for(int j = cross_index; j < gold_num; j++){
      o[i].gold_exist[j] = po[i%4].gold_exist[j];
    }
    if(!is_weight_under10(&o[i])) cross_index++;
    else{
      i++;
      cross_index = 1;
    }
  }
}

int evaluate_min_fit(Object* o){
  int min = o[0].fitness;
  for(int i = 0; i < population_size; i++){
    if(min > o[i].fitness) min = o[i].fitness;
  }
  return min;
}

int evaluate_max_fit(Object* o){
  int max = o[0].fitness;
  for(int i = 0; i < population_size; i++){
    if(max < o[i].fitness) max = o[i].fitness;
  }
  return max;
}

int evaluate_max_fit2(Object* o){
  int max = o[0].fitness;
  int max_index = 0;
  for(int i = 0; i < population_size; i++){
    if(max < o[i].fitness){
      max = o[i].fitness;
      max_index = i;
    }
  }
  return max_index;
}

void mutation(Object* o){
  while(1){
    srand(time(NULL));
    int i = rand()%population_size;
    int j = rand()%gold_num;
    if(o[i].gold_exist[j]) o[i].gold_exist[j] = 0;
    else o[i].gold_exist[j] = 1;
    if(!is_weight_under10(&o[i])){
      if(o[i].gold_exist[j]) o[i].gold_exist[j] = 0;
      else o[i].gold_exist[j] = 1;
    }
    else break;
  }
}

void print_population(Object* o){
  printf("-------------***-------------\n");
  for(int i = 0; i < population_size; i++){
    printf("%d. [ ", i);
    for(int j = 0; j < gold_num; j++){
      printf("%d ", o[i].gold_exist[j]);
    }
    printf("] fit : %d\n", o[i].fitness);
  }
}
