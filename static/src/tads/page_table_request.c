#include <tads/page_table_request.h>


t_page_table_request* create_page_table_request(){
	t_page_table_request* page_table_request = malloc(sizeof(t_page_table_request));
	page_table_request->table_number = 0;
	page_table_request->entry_number = 0;
	page_table_request->pid = 0;
	return page_table_request;
}

void page_table_request_destroy(t_page_table_request* page_table_request){
	free(page_table_request);
}





