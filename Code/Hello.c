
#include <linux/module.h>
#include <linux/slab.h>  //At first, lacks this 

MODULE_LICENSE("Dual BSD/GPL");

struct birthday{
	int day;
	int month;
	int year;

	struct list_head list;
};
struct birthday *entry;
struct birthday mylist;
struct birthday *tmp;

static int hello_init(void)
{
	int i, tmp_counter;
	INIT_LIST_HEAD(&mylist.list);
	tmp_counter = 1;
	
	printk("Loading Module\n");

	/*** Add elements into the linked lsit ***/
	for(i = 1; i <= 5; ++i){
		tmp = kmalloc(sizeof(struct birthday), GFP_KERNEL);
		tmp->day = i*6;
		tmp->month = i;
		tmp->year = 1999 + i;
		list_add_tail(&(tmp->list), &(mylist.list));
		printk("Add element %d\n", tmp_counter);
		++tmp_counter;
	}
	list_for_each_entry(tmp, &(mylist.list), list){
		//entry = list_entry(listptr, struct birthday, list);
		entry = tmp;
		printk("Birth: %d/%d/%d\n", entry->year, entry->month, entry->day);	

	}

	return 0;
}
static void hello_exit(void)
{
	int tmp_counter;
	tmp_counter = 1;
	struct birthday *prev = NULL;

	printk("Removing Module\n");
	/*** Free previous element in the loop to make list_for_each_entry work properly ****/
	list_for_each_entry(tmp, &(mylist.list), list){
		if(prev != NULL){
			list_del(&(prev->list));
			kfree(prev);
		}
		prev = tmp;
		printk("Delete element %d\n", tmp_counter);
		tmp_counter++;
	}

	if(prev != NULL){
		list_del(&(prev->list));
		kfree(prev);
	}

	/*********** Correctness Check ***********/
	list_for_each_entry(tmp, &(mylist.list), list){
		printk("Failed!!");
		entry = tmp;
                printk("Birth: %d/%d/%d\n", entry->year, entry->month, entry->day);

	}
	if(mylist.list.next == &(mylist.list)){//To check that whether kfree failed
		printk("freed properly\n");
	}
	else{
		printk("not freed properly\n");
	}
	/****************************************/
	printk("Goodbye!\n");
}

module_init(hello_init);
module_exit(hello_exit);

