
void cache_print(struct cache_t *cp /*cache pointer*/)
{       /* Simple Cache print procedure for Sim-cache, Simple Scalar Simulator*/
	/*Author: Shrivinayak Bhat, University of Florida*/

/*      if (strcmp(cp->name,"data1")!=0) return;   //use this to filter by cache name */
        printf("\n\n");
        printf("cache: %s\n",cp->name);

        /*printing metadata*/
        printf("assoc: %d\n",cp->assoc);
        printf("nsets: %d\n",cp->nsets);
        printf("bsize: %d\n",cp->bsize);

        /*declarations*/
        struct cache_blk_t *blk ; /*blk pointer variable to iterate through the blocks*/
        struct cache_set_t *set ; /*set pointer variable to iterate through the sets*/
        int j;
        char *ptr; /*char pointer to load data from blocks*/

        /*iterating through the sets of a cache*/
        for(int i=0;i<cp->nsets;i++)
        {
                printf("\nset: %d\n",i);

                set = &(cp->sets[i]);
                blk = set->way_head;
                j=0;

                /* iterating via blocks of a set*/
                while(blk){
                        printf("block no: %d\t",j++);
                        printf("tag: %d\t",(blk->tag));

                        /*printing block status*/
                        if(blk->status == 1)
                                printf("status: valid\t");
                        else if(blk->status ==2)
                                printf("status: dirty\t");
                        else
                        printf("status: invalid\t");

                        ptr=blk->data;
                        printf("user_data: ");
                        int limit=cp->bsize/sizeof(char);
                        int count=0;

                        /*printing data stored in the block - junk value since endianess is not taken care off*/
                        while(count++<=limit)
                                printf("%d",*(ptr++));
                        printf("\n");
                        /*move to the next block in the chain*/
                        blk = blk->way_next;
                }
        }

        printf("\n\n");

}
