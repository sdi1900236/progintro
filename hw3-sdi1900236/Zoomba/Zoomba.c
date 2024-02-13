#include<stdio.h>
#include<stdlib.h>

struct node {
int x;
int y;
int depth;
struct node * parent;
struct node * left;
struct node * right;
struct node * up;
struct node * down;
};

struct FRONT {
struct node * node;
struct FRONT * next;
};

typedef struct FRONT front;
typedef struct node node;

node * initialize_node ( int x, int y );
front * initialize_front ( node * nd );
node * add ( node * nd, int x, int y );
node * push ( node * nd, int x, int y );
front * push_in_front ( front * frnt, node * nd );
int check_blockage ( int x, int y, int N, int ** map );
int target_met ( node * nd, int target_loctn_x, int target_loctn_y );
node * tree_search ( front * frnt, int N, int ** map, int ** cost, int target_loctn_x, int target_loctn_y );
front * expand ( front * frnt, node * nd, int N, int ** map );
void free_front ( front * frnt );
void free_node ( node * nd );

int main ( int argc, char ** argv )
{
  FILE * file;
  int N, init_loctn_x, init_loctn_y, target_loctn_x, target_loctn_y, i, j, ** cost, **map, depth;
  char ch, * output;
  node * nd;
  front * frnt;

  /* Make sure an input file is given - exit otherwise */
  if ( argc < 2 )
  {
    printf("No input file given\n");
    exit(0);
  }

  /* Open input file */
  if ( ( file = fopen ( argv[1], "r" ) ) == NULL )
  {
    printf("Input file not found\n");
    exit(0);
  }

  /* Read N */
  fscanf ( file, "%d", &N );
  /* Allocate memoty for map and cost matrices */
  map = malloc ( N * sizeof ( int * ) );
  cost = malloc ( N * sizeof ( int * ) );

  /* Initialize map and cost matrices */
  for ( i = 0; i < N; i++ )
  {
    map[i] = malloc ( N * sizeof ( int ) );
    cost[i] = malloc ( N * sizeof ( int ) );
  }

  /* Read initial and target locations */
  fscanf ( file, "%d %d %d %d", &init_loctn_x,  &init_loctn_y, &target_loctn_x, &target_loctn_y );

  /* Convert map[i][j] to 0/1 (integer) */
  for ( i = 0; i < N; i++ )
  {
    fscanf( file, "%c", &ch );
    for ( j = 0; j < N; j++ )
    {
      fscanf ( file, "%c", &ch );
      map[i][j] = atoi ( &ch );
      cost[i][j] = N * N;
    }
  }

  /* Close the input file */
  fclose ( file );

  /* Check that the input data is correct - exit otherwise */
  if ( check_blockage ( init_loctn_x, init_loctn_y, N, map ) == 0 )
    exit(1);

  /* Initialize parent node to initial point */
  nd = initialize_node ( init_loctn_x, init_loctn_y );

  /* Initialize front to point to initial node */
  frnt = initialize_front ( nd );

  /* Run main algorithm (tree search */
  nd = tree_search ( frnt, N, map, cost, target_loctn_x, target_loctn_y );

  /* If NULL is returned, the maze is impossible to solve */
  if ( nd == NULL )
  {
    printf("0\n");
    exit(0);
  }

  /* Save depth (minimum distance to target) */
  depth = nd->depth;

  /* Allocate memory for output sequence so that we can later reverse it */
  output = malloc ( depth * sizeof ( char ) );

  /* Extract directins information from final node going upwards using parent nodes */
  for ( i = 0; i < depth; i++ )
  {
    if ( nd->parent->x < nd->x ) output[i] = 'D';
    else if ( nd->parent->x > nd->x ) output[i] = 'U';
    else if ( nd->parent->y < nd->y ) output[i] = 'R';
    else if ( nd->parent->y > nd->y ) output[i] = 'L';
    nd = nd->parent;
  }
  
  /* Free paths */
  free_node ( nd );

  /* Print output (in reverse order) */
  for ( i = depth - 1; i >= 0; i-- )
  printf("%c", output[i] );
  printf("\n");

  /* Free other momery used */
  for ( i = 0; i < N; i++ )
    free ( map[i] );
  free ( map );
  for ( i = 0; i < N; i++ )
    free ( cost[i] );
  free ( cost );
  free ( output );
  exit(1);
}

front * initialize_front ( node * nd )
{
  front * frnt;
  frnt = malloc ( sizeof ( front ) );
  frnt->node = nd;
  frnt->next = NULL;

  return frnt;
}

node * initialize_node ( int x, int y )
{
  node * result = malloc ( sizeof ( node ) );
  result->parent = NULL;
  result->x = x;
  result->y = y;
  result->depth = 0;
  result->left = NULL;
  result->right = NULL;
  result->up = NULL;
  result->down = NULL;

  return result;
}

node * push ( node * nd, int x, int y )
{
  node * nd1;

  nd1 = malloc ( sizeof ( node ) );
  nd1->x = x;
  nd1->y = y;
  nd1->depth = nd->depth + 1;
  nd1->parent = nd;
  nd1->left = NULL;
  nd->right = NULL;
  nd->up = NULL;
  nd->down = NULL;

  return nd1;
}

front * push_in_front ( front * frnt, node * nd )
{
  front * frnt1 = malloc ( sizeof ( front *) );
  frnt1->next = frnt;
  frnt1->node = nd;

  return frnt1;
}

void free_node ( node * nd )
{
  node * nd1 = nd;

  if ( nd1->left != NULL )
    free_node ( nd1->left );
  if ( nd1->right != NULL )
    free_node ( nd1->right );
  if ( nd1->up != NULL )
    free_node ( nd1->up );
  if ( nd1->down != NULL )
    free_node ( nd1->down );

    free ( nd1 );
}

void free_front ( front * frnt )
{
  front * frnt1 = frnt, * frnt2;

  while ( frnt1 != NULL )
  {
    frnt2 = frnt1->next;
    free ( frnt1 );
    frnt1 = frnt2;
  }
}

int check_blockage ( int x, int y, int N, int ** map )
{
  /* Here we check if the directions of a requested move are either outside the maze or on a blocked square */
  if ( ( x < 0 ) || ( x >= N ) || ( y < 0 ) || ( y >= N ) )
    return 0;
  else if ( map[x][y] == 1 ) return 0; else return 1;
}

int target_met ( node * nd, int target_loctn_x, int target_loctn_y )
{
  /* Here we check if we have found the target */
  if ( ( nd->x == target_loctn_x ) && ( nd->y == target_loctn_y ) ) return 1;
  else return 0;
}

node * tree_search ( front * frnt, int N, int ** map, int ** cost, int target_loctn_x, int target_loctn_y )
{
  front * frnt_cur = frnt, * frnt_new = NULL;
  node * nd = frnt->node;
  int expansion;

  do /* Main loop for each front expansion */
  {
    expansion = 0;

    /* Loop of expansion of all nodes in the current front */
    while ( frnt_cur != NULL )
    {
      nd = frnt_cur->node;

      /* Check if the target is met - then the target is met and the function returns the final node */
      if ( target_met ( nd, target_loctn_x, target_loctn_y ) ) return frnt_cur->node;
        /* Check if the cost of the solution is < the current cost of the specific square, i.e. if this is the fastest path to this square */
        else if ( cost[nd->x][nd->y] > nd->depth )
	{
          /* If it is, then update cost and expand */
	  cost[nd->x][nd->y] = nd->depth;
	  frnt_new = expand ( frnt_new, nd, N, map );
	  expansion = 1;
	}
      frnt_cur = frnt_cur->next;
    }

    /* Free current front list, we no longer need it */    
    free_front ( frnt_cur );

    /* Assign current point to the new front */
    frnt_cur = frnt_new;
  } while ( expansion == 1 ); /* If expansion = 0 then no expansion has been made, i.e. we are in a deadend, i.e, the maze is impossible */

  /* Free the last front list */
  free_front ( frnt_cur );

  return NULL;
}

front * expand ( front * frnt, node * nd, int N, int ** map )
{
  front * frnt1 = frnt;

  /* Expand left */
  /* Check blockage first */
  if ( check_blockage ( nd->x, nd->y - 1, N, map ) )
  {
    /* Push new element to the left */
    nd->left = push ( nd, nd->x, nd->y - 1 );
    /* Push new node in the front */
    frnt1 = push_in_front ( frnt1, nd->left );
  }
  /* Expand right */
  /* Check blockage first */
  if ( check_blockage ( nd->x, nd->y + 1, N, map ) )
  {
    /* Push new element to the right */
    nd->right = push ( nd, nd->x, nd->y + 1 );
    /* Push new node in the front */
    frnt1 = push_in_front ( frnt1, nd->right );
  }

  /* Expand up */
  /* Check blockage first */
  if ( check_blockage ( nd->x - 1, nd->y, N, map ) )
  {
    /* Push new element up */
    nd->up = push ( nd, nd->x - 1, nd->y );
    /* Push new node in the front */
    frnt1 = push_in_front ( frnt1, nd->up );
  }
  /* Expand down */
  /* Check blockage first */
  if ( check_blockage ( nd->x + 1, nd->y, N, map ) )
  {
    /* Push new element down */
    nd->down = push ( nd, nd->x + 1, nd->y );
    /* Push new node in the front */
    frnt1 = push_in_front ( frnt1, nd->down );
  }

  return frnt1;
}
