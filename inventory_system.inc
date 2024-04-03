/* Требуется YSI для перехвата функций (https://github.com/pawn-lang/YSI-Includes) */

#if defined inventorysystem__included
    #endinput
#endif
#define inventorysystem__included

#if !defined _mdialog_included
	#error "Please included <mdialog> before include <inventory_system>"
#endif

#if !defined foreach
    #error "Please included <foreach> before include <inventory_system>
#endif

#if !defined sscanf
    #error "Please included <sscanf> before include <inventory_system>
#endif

#if !defined mysql_included || !defined cache_get_value_name
    #error "Please included <mysql> ver. R40+ before include <inventory_system>
#endif

/*
	Macroses
*/
#define MAX_ITEMS_FOR_PLAYER				(50) 	/* Максимальное количество предметов на ОДНОГО игрока */
#define MAX_ITEMS							(35) 	/* Все предметы, которые могут быть добавлены в инвентарь игрока */
#define MIN_INVENTORY_PAGE					(0) 	/* Минимальная страница. Не спрашивайте зачем оно тут, в дальнейшем может быть пригодится */
#define MAX_INVENTORY_PAGE					(3) 	/* Максимальная страница */
#define ITEM_TYPE_NONE						(-1)	/* Неопределённый тип предмета */
#define ITEM_TYPE_GUN						(0) 	/* Тип предмета - ОРУЖИЕ */
#define ITEM_TYPE_FOOD						(1) 	/* Тип предмета - ПИЩА */
#define ITEM_TYPE_DRINKS					(2) 	/* Тип предмета - НАПИТКИ */
#define ITEM_TYPE_CLOTHING					(3) 	/* Тип предмета - ОДЕЖДА */
#define ITEM_TYPE_DRUGS						(4) 	/* Тип предмета - НАРКОТИКИ */
#define ITEM_TYPE_MEDICAL					(5) 	/* Тип предмета - МЕДИЦИНА */
#define MAX_ITEM_COUNT						(50)	/* Максимальное количество предмета, которое можно выдать/получить/подобрать */
#define MAX_ITEM_NAME_LENGHT				(32) 	/* Максимальная длина для предмета */
#define GetItemData(%0,%1)					items_data[%0][%1]
#define GetInventoryData(%0,%1,%2,%3) 		inventory_player_data[%0][%1][%2][%3]
#define SetInventoryData(%0,%1,%2,%3,%4) 	inventory_player_data[%0][%1][%2][%3] = %4

/*
	Enums
*/
enum E_PLAYER_INVENTORY_STRUCT {
	ITEM_SLOT,
	ITEM_COUNT,
	ITEM_ID
};

enum E_ITEMS_STRUCT {
	ITEM_MODEL,
	ITEM_NAME[MAX_ITEM_NAME_LENGHT],
	ITEM_TYPE,
	Float: ITEM_DISPOSITION_X,
	Float: ITEM_DISPOSITION_Y,
	Float: ITEM_DISPOSITION_Z,
	Float: ITEM_DISPOSITION_R
};

/*
	Variables
*/
new inventory_player_data[MAX_PLAYERS][MAX_INVENTORY_PAGE][MAX_ITEMS_FOR_PLAYER][E_PLAYER_INVENTORY_STRUCT];
new Iterator: IterInventory[MAX_PLAYERS]<MAX_ITEMS_FOR_PLAYER>;
new items_data[MAX_ITEMS][E_ITEMS_STRUCT] = {
	{ 0, " ", ITEM_TYPE_NONE, 0.0, 0.0, 0.0, 0.0 },
	{ 337, "Лопата", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 0.0 },
	{ 341, "Бензопила", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 0.0 },
	{ 321, "ДИЛДАК", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 0.0 }
};

/*
	Functions
*/
stock initializationInventoryIterator() {
	Iter_Init(IterInventory);
	return true;
}

stock populateWithInventoryItems(playerid, inventory_page = 0) {
	if (!IsPlayerConnected(playerid)) return true;
	
	new fmt[] = "Страница предмета: %d; Слот предмета: %d; Количество предмета: %d; ID предмета: %d;";
	new fmt_size[sizeof fmt + (-2+1) + (-2+2) + (-2+2) + (-2+3)];
	
	foreach (new i: IterInventory[playerid]) {
		new 
			slot = GetInventoryData(playerid, inventory_page, i, ITEM_SLOT),
			count = GetInventoryData(playerid, inventory_page, i, ITEM_COUNT),
			id = GetInventoryData(playerid, inventory_page, i, ITEM_ID)
		;

		if (!id)
			continue;
		
		format(fmt_size, sizeof fmt_size, fmt, inventory_page, slot, count, id);
		SendClientMessage(playerid, -1, fmt_size);
	}
	
	return true;
}

cmd:add_item(playerid, params[]) {
	new page, slot, count, id;
	if (sscanf(params, "dddd", page, slot, count, id))
		return SendClientMessage(playerid, -1, "/add_item <page> <slot> <count <item_id>");

	if ((id < 0 || id > sizeof items_data))
		return SendClientMessage(playerid, -1, "Укажите валидный предмет.");

	if ((page < 0 || page > MAX_INVENTORY_PAGE))
		return SendClientMessage(playerid, -1, "Укажите валидную страницу.");

	if ((count < 0 || count > MAX_ITEM_COUNT))
		return SendClientMessage(playerid, -1, "Укажите валидное количество предмета. От 0 до 50-и.");

	new free_index = Iter_Free(IterInventory[playerid]);
	if (free_index == ITER_NONE) return print("> Inventory full.");

	SetInventoryData(playerid, page, free_index, ITEM_SLOT, slot);
	SetInventoryData(playerid, page, free_index, ITEM_COUNT, count);
	SetInventoryData(playerid, page, free_index, ITEM_ID, id);

	Iter_Add(IterInventory[playerid], free_index);
	return SendClientMessage(playerid, -1, "Success");
}

cmd:items(playerid, params[]) {
	new page = 0;
	if (sscanf(params, "d", page)) 
		return SendClientMessage(playerid, -1, "/items <page>");

	if ((page < 0 || page > MAX_INVENTORY_PAGE))
		return SendClientMessage(playerid, -1, "Укажите валидную страницу.");
	
	return populateWithInventoryItems(playerid, page);
}

/* Hooks */

hook OnGameModeInit() {
	initializationInventoryIterator();
	return true;
}


