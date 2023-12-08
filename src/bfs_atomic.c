//Stub for custom BFS implementations

#include "common.h"
#include "aml.h"
#include "csr_reference.h"
#include "bitmap_reference.h"
#include <stdint.h>
#include <inttypes.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#include <omp.h>

atomic_long *pred_atomic;
int *queue, *next_queue;
int queue_ptr;
atomic_int next_queue_ptr;

//VISITED bitmap parameters
unsigned long *visited;
int64_t visited_size;

int64_t *pred_glob,*column;
int *rowstarts;
oned_csr_graph g;

//user should provide this function which would be called once to do kernel 1: graph convert
void make_graph_data_structure(const tuple_graph* const tg) {
	//graph conversion, can be changed by user by replacing oned_csr.{c,h} with new graph format
	int i;
    convert_graph_to_oned_csr(tg, &g);
	column = g.column;
	rowstarts = g.rowstarts;

	pred_atomic = xmalloc(g.nlocalverts * sizeof(long));
	queue = xmalloc(g.nlocalverts * sizeof(int)); //100% of vertexes
	next_queue = xmalloc(g.nlocalverts * sizeof(int));

	for (i = 0; i < g.nlocalverts; i++) {
		atomic_store(&pred_atomic[i], -1);
		queue[i] = -1;
		next_queue[i] = -1;
	}
	//user code to allocate other buffers for bfs
}

//user should provide this function which would be called several times to do kernel 2: breadth first search
//pred[] should be root for root, -1 for unrechable vertices
//prior to calling run_bfs pred is set to -1 by calling clean_pred
void run_bfs(int64_t root, int64_t* pred, int num_threads) {
	pred_glob=pred;
	
	queue_ptr = 0;
	atomic_store(&next_queue_ptr, 0);
    atomic_store(&pred_atomic[root], root);
    queue[queue_ptr] = root;
    queue_ptr++;

    while (queue_ptr) {
        int i;
        #pragma omp parallel for num_threads(num_threads)
        for (i = 0; i < queue_ptr; i++) {
            int current_root = queue[i];

            int j;
            for (j = rowstarts[queue[i]]; j < rowstarts[queue[i] + 1]; j++) {
                int neighbor = COLUMN(j);
				long invalid_val = -1;
				if (atomic_compare_exchange_strong(&pred_atomic[neighbor], &invalid_val, current_root)) {
					int next_queue_ptr_before_add = atomic_fetch_add(&next_queue_ptr, 1);
					next_queue[next_queue_ptr_before_add] = neighbor;
				}
            }
        }

        queue_ptr = atomic_load(&next_queue_ptr);
        atomic_store(&next_queue_ptr, 0);
        int *temp = queue; queue = next_queue; next_queue = temp;
    }

	int k;
	for (k = 0; k < g.nlocalverts; k++) {
		pred[k] = atomic_load(&pred_atomic[k]);
	}
}

//we need edge count to calculate teps. Validation will check if this count is correct
//user should change this function if another format (not standart CRS) used
void get_edge_count_for_teps(int64_t* edge_visit_count) {
	long i,j;
	long edge_count=0;
	for(i=0;i<g.nlocalverts;i++)
		if(pred_glob[i]!=-1) {
			for(j=g.rowstarts[i];j<g.rowstarts[i+1];j++)
				if(COLUMN(j)<=VERTEX_TO_GLOBAL(my_pe(),i))
					edge_count++;
		}
	aml_long_allsum(&edge_count);
	*edge_visit_count=edge_count;
}

//user provided function to initialize predecessor array to whatevere value user needs
void clean_pred(int64_t* pred) {
	int i;
	for(i=0;i<g.nlocalverts;i++) {
		atomic_store(&pred_atomic[i], -1);
		pred[i]=-1;
	}
}

//user provided function to be called once graph is no longer needed
void free_graph_data_structure(void) {
	free_oned_csr_graph(&g);
	free(visited);
}

//user should change is function if distribution(and counts) of vertices is changed
size_t get_nlocalverts_for_pred(void) {
	return g.nlocalverts;
}
