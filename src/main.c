#include "asm.h"

s_container_arr* container_s;
float power_consumption = 0;
float power_output = 40.0f;
int log_power = 0;

//Main menu loop.
int main() {
	container_s = alloc_struct();
	if(container_s == NULL)
		return 1;

	printf("%s", APP_CLEAR);
	int opt;
	do {
		printf("Power usage: %.1f / %.1f KWh\n", power_consumption, power_output);

		//Checks if power is being consumed.
		if(power_consumption > 0) {
			//Checks if the power consumption is greater than the output.
			if(power_consumption > power_output)
				printf("%s", RED("Power output is insuficient.\n\n"));
			else
				printf("%s", GREEN("Power consumption is stable.\n\n"));
		}
		else
			printf("No containers have been imported.\n\n");

		printf("%s", MENU_STR);
		printf("Option: ");
		scanf("%d", &opt);
		printf("%s", APP_CLEAR);

		switch(opt) {
			case 1:
				container_parse(CONTAINERS_FULL);
				break;
			case 2:
				container_parse(CONTAINERS_DOUBLE);
				break;
			case 3:
				container_parse(CONTAINERS_SINGLE);
				break;
			case 4:
				print_containers();
				break;
			case 5:
				checkContainerRefrigerated();
				break;
			case 6:
				alterPowerOutput();
				break;
			case 7:
				if(log_power == 0) {
					log_power = 1;
					printf("Power log enabled.\n\n");
				}
				else {
					log_power = 0;
					printf("Power log disabled.\n\n");
				}
				break;
			case 8:
				reset_array();
				break;
			case 0:
				if(container_s->used != 0)
					free(container_s->container_array);
				free(container_s);
				break;
			default:
				printf("%s", YELLOW("Invalid option.\n\n"));
				break;
		}
	} while(opt != 0);
	return 0;
}

//Allocates memory for the main structure and assings the default values.
s_container_arr* alloc_struct() {
	s_container_arr* container_arr = malloc(sizeof(s_container_arr));
	if(container_arr == NULL)
		return NULL;

	container_arr->used = 0;
	container_arr->max = X*Y*Z;
	return container_arr;
}

//Counts the number of lines in a file.
int count_lines(char* path) {
	FILE* fp = fopen(path, "r");
	int count = 0;

	if(fp) {
		char c;
		for(c = getc(fp); c != EOF; c = getc(fp))
			if (c == '\n')
				count++;  //Increments the number of lines everytime it finds a new line character.
	}
	return count;
}

//Handles the parsing of the Container's CSV file.
void container_parse(char* path) {
	int lines = count_lines(path);
	if(lines > container_s->max) {
		printf("%s", RED("The ship doesn't have enough capacity.\n\n"));
		return;
	}

	if(alloc_array(lines) == 1)
		return;

	FILE* fp = fopen(path, "r");

	if(!fp)
		printf("%s", RED("Failed to open file.\n\n"));
    else {
		int column;
		char line[16];

		while(fgets(line, 16, fp)) {          //Reads the next line from the file.

			column = 1;                       //Resets the column counter.
			char* value = strtok(line, ",");  //Separates the values using the comma.

			while (value) {
				switch(column) {
					case 1:
						container_s->container_array[container_s->used].id = atoi(value);    //Container ID
						break;
					case 2:
						container_s->container_array[container_s->used].x = atoi(value);     //X axis coordinate
						break;
					case 3:
						container_s->container_array[container_s->used].y = atoi(value);     //Y Axis coordinate
						break;
					case 4:
						container_s->container_array[container_s->used].z = atoi(value);     //Z Axis coordinate
						break;
				}
                column++;  //Increments the column number.
                value = strtok(NULL, ",");
            }
            container_s->used++;  //Increments the array counter.
		}
		if(container_s->used != 0) {
			container_data_parse(DATA_CONTAINERS);
			calculate_power_consumption();
			printf("Containers imported successfully.\n\n");
		}
		else {
			printf("%s", RED("Failed to import containers.\n\n"));
		}
	}
}

//Handles the parsing of the Container's data CSV file.
void container_data_parse(char* path) {
	FILE* fp = fopen(path, "r");

	if(!fp)
		printf("%s", RED("Failed to open data file.\n\n"));
    else {
		int column;
		char line[48];

		while(fgets(line, 48, fp)) {          //Reads the next line from the file.

			column = 1;                       //Resets the column counter.
			char* value = strtok(line, ",");  //Separates the values using the comma.
			int index;

			while (value) {
				switch(column) {
					case 1:
						index = getContainerIndexFromID(atoi(value));  //Finds the index of the container if it exists.
						break;
					case 2:
						if(index != -1)
							container_s->container_array[index].length = atof(value);   //Length
						break;
					case 3:
						if(index != -1)
							container_s->container_array[index].width = atof(value);    //Width.
						break;
					case 4:
						if(index != -1)
							container_s->container_array[index].height = atof(value);   //Height
						break;
					case 5:
						if(index != -1)
							container_s->container_array[index].type = atoi(value);     //Type
						break;
					case 6:
						if(index != -1)
							container_s->container_array[index].f_mat = atoi(value);    //First material
						break;
					case 7:
						if(index != -1)
							container_s->container_array[index].s_mat = atoi(value);    //Second material
						break;
					case 8:
						if(index != -1)
							container_s->container_array[index].t_mat = atoi(value);    //Third material
						break;
					case 9:
						if(index != -1)
							container_s->container_array[index].f_depth = atof(value);  //Depth of the first material
						break;
					case 10:
						if(index != -1)
							container_s->container_array[index].s_depth = atof(value);  //Depth of the second material
						break;
					case 11:
						if(index != -1)
							container_s->container_array[index].t_depth = atof(value);  //Depth of the third material
						break;
				}
                column++;  //Increments the column number.
                value = strtok(NULL, ",");
            }
		}
	}
}

//Resets the counter back to zero and liberates the allocated memory.
void reset_array() {
	power_consumption = 0;
	if(container_s->used == 0)
		printf("Nothing to clear.\n\n");
	else {
		container_s->used = 0;
		free(container_s->container_array);
		printf("The array was reset and the memory was cleared.\n\n");
	}
}

//Allocates or Reallocates memory for the array of containers.
int alloc_array(int size) {
	if(container_s->used == 0) {
		container_s->container_array = malloc(sizeof(s_container) * size);
		if(container_s->container_array == NULL) {
			printf("%s", RED("Failed to allocate memory for the array.\n\n"));
			return 1;
		}
		return 0;
	}
	else {
		if(container_s->used != size) {
			container_s->container_array = realloc(container_s->container_array, sizeof(s_container) * size);
			if(container_s->container_array == NULL) {
				printf("%s", RED("Failed to reallocate memory for the array.\n\n"));
				return 1;
			}
		}
		container_s->used = 0;
		return 0;
	}
}

//Prints all the data from every container.
void print_containers() {
	if(container_s->used == 0)
		printf("%s", YELLOW("Import data first.\n\n"));
	else {
		printf("ID | X | Y |  Z | Length | Width | Height | Type | F_Mat | S_Mat | T_Mat | F_Depth | S_Depth | T_Depth\n\n");
		for(int i = 0; i < container_s->used; i++) {
			if(i == 5 || i == 14 || i == 23)
				printf("\x1b[33m");
			printf("%s%d | %d | %d | %s%d |   %.2f |  %.2f |   %.2f |    %d |     %d |     %d |     %d |   %.3f |   %.3f |   %.3f\n",
				container_s->container_array[i].id < 10 ? " " : "",
				container_s->container_array[i].id,
				container_s->container_array[i].x,
				container_s->container_array[i].y,
				container_s->container_array[i].z >= 0 ? " " : "",
				container_s->container_array[i].z,
				container_s->container_array[i].length,
				container_s->container_array[i].width,
				container_s->container_array[i].height,
				container_s->container_array[i].type,
				container_s->container_array[i].f_mat,
				container_s->container_array[i].s_mat,
				container_s->container_array[i].t_mat,
				container_s->container_array[i].f_depth,
				container_s->container_array[i].s_depth,
				container_s->container_array[i].t_depth);
			if(i == 5 || i == 14 || i == 23)
				printf("\x1b[0m");
		}
		printf("\n");
	}
}

//Calculates the power consumption of all the refrigerated containers.
void calculate_power_consumption() {
	power_consumption = 0;
	for(int i = 0; i < container_s->used; i++) {
		if(check_type(container_s->container_array, i) == 1) {
			float area = container_s->container_array[i].length * container_s->container_array[i].height * 4 +
			container_s->container_array[i].width * container_s->container_array[i].height * 2;
			float resistance = (container_s->container_array[i].f_depth / (getMaterialResistanceFromID(container_s->container_array[i].f_mat) * area)) +
			(container_s->container_array[i].s_depth / (getMaterialResistanceFromID(container_s->container_array[i].s_mat) * area)) +
			(container_s->container_array[i].t_depth / (getMaterialResistanceFromID(container_s->container_array[i].t_mat) * area));
			float container_consumption = (20 - (container_s->container_array[i].type == 1 ? 5 : -7)) / resistance;
			if(log_power == 1)
				printf("Container ID: %d\nArea: %.2f m2\nResistance: %f Ohms\nPower Consumption: %.2f W\n\n", container_s->container_array[i].id, area, resistance, container_consumption);
			power_consumption += container_consumption;
		}
	}
	power_consumption = power_consumption / 1000;
}

//Finds the index of a container given an ID.
int getContainerIndexFromID(int id) {
	for(int i = 0; i < container_s->used; i++)
		if(container_s->container_array[i].id == id)
			return i; //Index of the container.

	return -1; //The container doesn't exist.
}

//Returns the resistance of a material given an ID.
float getMaterialResistanceFromID(int id) {
	switch(id) {
		case 1:
			return 80.4f;
		case 2:
			return 0.11f;
		case 3:
			return 0.52f;
		default:
			return 0;
	}
}

//Checks if a container is refrigerated or not.
void checkContainerRefrigerated() {
	int id, index;

	if(container_s->used == 0) {
		printf("%s", YELLOW("Import data first.\n\n"));
		return;
	}

	printf("Container ID: ");
	scanf("%d", &id);

	index = getContainerIndexFromID(id);

	if(index != -1)
		printf("The container %s refrigerated.\n\n", check_type(container_s->container_array, index) == 1 ? "IS" : "IS NOT");
	else
		printf("%s", YELLOW("Container doesn't exist.\n\n"));
}

//Alters the ship's current power output.
void alterPowerOutput() {
	printf("Power Output (KWh): ");
	scanf("%f", &power_output);
	printf("Power output updated successfully.\n\n");
}
