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
#define MAX_ITEMS							(4) 									
#define MIN_INVENTORY_PAGE					(0) 									
#define MAX_INVENTORY_PAGE					(3) 											
#define ITEM_TYPE_NONE						(-1)												
#define ITEM_TYPE_GUN						(0) 												
#define ITEM_TYPE_FOOD						(1) 												
#define ITEM_TYPE_DRINKS					(2) 												
#define ITEM_TYPE_CLOTHING					(3) 												
#define ITEM_TYPE_DRUGS						(4) 												
#define ITEM_TYPE_MEDICAL					(5) 												
#define MAX_ITEM_COUNT						(50)												
#define MAX_ITEM_NAME_LENGHT				(32) 												
#define INVENTORY_SIZE_LENGHT				(5)													
#define INVENTORY_SIZE_HEIGHT				(4)
#define INVALID_INVENTORY_PAGE				(-1)
#define INVALID_INVENTORY_CLICK_SLOT		(-1)													
#define INVENTORY_SIZE						(INVENTORY_SIZE_LENGHT * INVENTORY_SIZE_HEIGHT)
#define MAX_ITEMS_FOR_PLAYER				(INVENTORY_SIZE)
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
new Text: g_global_textdraws[8];
new PlayerText: g_player_textdraw_slots[MAX_PLAYERS][INVENTORY_SIZE];
new inventory_player_data[MAX_PLAYERS][MAX_INVENTORY_PAGE][MAX_ITEMS_FOR_PLAYER][E_PLAYER_INVENTORY_STRUCT];
new Iterator: IterInventory[MAX_PLAYERS]<MAX_ITEMS_FOR_PLAYER>;
new bool: is_open_inventory[MAX_PLAYERS] = {false, ...};
new g_player_page[MAX_PLAYERS] = {INVALID_INVENTORY_PAGE, ...};
new g_player_clicked_slot[MAX_PLAYERS] = {INVALID_INVENTORY_CLICK_SLOT, ...};
new items_data[MAX_ITEMS][E_ITEMS_STRUCT] = {
	{ 18631, " ", ITEM_TYPE_NONE, 0.0, 0.0, 0.0, 0.0 },
	{ 337, "Лопата", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 3.0 },
	{ 341, "Бензопила", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 3.0 },
	{ 321, "ДИЛДАК", ITEM_TYPE_GUN, 0.0, 0.0, 0.0, 3.0 }
};

/*
	Functions
*/
stock bool: IsOpenInventory(playerid) {
	return is_open_inventory[playerid];
}

stock CreateGlobalInventoryTextDraws() {
	g_global_textdraws[0] = TextDrawCreate(455.9993, 100.8147, "X");
	TextDrawLetterSize(g_global_textdraws[0], 0.3362, 1.3054);
	TextDrawTextSize(g_global_textdraws[0], 465.0000, 2.9999);
	TextDrawAlignment(g_global_textdraws[0], 1);
	TextDrawColor(g_global_textdraws[0], -16776961);
	TextDrawUseBox(g_global_textdraws[0], 1);
	TextDrawBoxColor(g_global_textdraws[0], 0);
	TextDrawSetOutline(g_global_textdraws[0], 1);
	TextDrawBackgroundColor(g_global_textdraws[0], 255);
	TextDrawFont(g_global_textdraws[0], 1);
	TextDrawSetProportional(g_global_textdraws[0], 1);
	TextDrawSetShadow(g_global_textdraws[0], 1);
	TextDrawSetSelectable(g_global_textdraws[0], true);

	g_global_textdraws[1] = TextDrawCreate(457.0000, 371.9331, "Box");
	TextDrawLetterSize(g_global_textdraws[1], 0.0000, 2.1998);
	TextDrawTextSize(g_global_textdraws[1], 502.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[1], 1);
	TextDrawColor(g_global_textdraws[1], -1);
	TextDrawUseBox(g_global_textdraws[1], 1);
	TextDrawBoxColor(g_global_textdraws[1], 150);
	TextDrawBackgroundColor(g_global_textdraws[1], 255);
	TextDrawFont(g_global_textdraws[1], 1);
	TextDrawSetProportional(g_global_textdraws[1], 1);
	TextDrawSetShadow(g_global_textdraws[1], 0);

	g_global_textdraws[2] = TextDrawCreate(570.2338, 372.0775, "Box");
	TextDrawLetterSize(g_global_textdraws[2], 0.0000, 2.1830);
	TextDrawTextSize(g_global_textdraws[2], 613.6998, 0.0000);
	TextDrawAlignment(g_global_textdraws[2], 1);
	TextDrawColor(g_global_textdraws[2], -1);
	TextDrawUseBox(g_global_textdraws[2], 1);
	TextDrawBoxColor(g_global_textdraws[2], 150);
	TextDrawBackgroundColor(g_global_textdraws[2], 255);
	TextDrawFont(g_global_textdraws[2], 1);
	TextDrawSetProportional(g_global_textdraws[2], 1);
	TextDrawSetShadow(g_global_textdraws[2], 0);

	g_global_textdraws[3] = TextDrawCreate(504.8476, 371.8775, "Box");
	TextDrawLetterSize(g_global_textdraws[3], 0.0000, 2.1998);
	TextDrawTextSize(g_global_textdraws[3], 567.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[3], 1);
	TextDrawColor(g_global_textdraws[3], -1);
	TextDrawUseBox(g_global_textdraws[3], 1);
	TextDrawBoxColor(g_global_textdraws[3], 150);
	TextDrawBackgroundColor(g_global_textdraws[3], 255);
	TextDrawFont(g_global_textdraws[3], 1);
	TextDrawSetProportional(g_global_textdraws[3], 1);
	TextDrawSetShadow(g_global_textdraws[3], 0);

	g_global_textdraws[4] = TextDrawCreate(504.8476, 371.8775, "Box");
	TextDrawLetterSize(g_global_textdraws[4], 0.0000, 2.1998);
	TextDrawTextSize(g_global_textdraws[4], 567.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[4], 1);
	TextDrawColor(g_global_textdraws[4], -1);
	TextDrawUseBox(g_global_textdraws[4], 1);
	TextDrawBoxColor(g_global_textdraws[4], 150);
	TextDrawBackgroundColor(g_global_textdraws[4], 255);
	TextDrawFont(g_global_textdraws[4], 1);
	TextDrawSetProportional(g_global_textdraws[4], 1);
	TextDrawSetShadow(g_global_textdraws[4], 0);

	g_global_textdraws[5] = TextDrawCreate(505.6666, 365.4666, "use");
	TextDrawLetterSize(g_global_textdraws[5], 0.2522, 2.0397);
	TextDrawTextSize(g_global_textdraws[5], 519.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[5], 1);
	TextDrawColor(g_global_textdraws[5], -1);
	TextDrawUseBox(g_global_textdraws[5], 1);
	TextDrawBoxColor(g_global_textdraws[5], 0);
	TextDrawSetOutline(g_global_textdraws[5], 1);
	TextDrawBackgroundColor(g_global_textdraws[5], 255);
	TextDrawFont(g_global_textdraws[5], 1);
	TextDrawSetProportional(g_global_textdraws[5], 1);
	TextDrawSetShadow(g_global_textdraws[5], 0);
	TextDrawSetSelectable(g_global_textdraws[5], true);

	g_global_textdraws[6] = TextDrawCreate(505.1659, 378.4374, "drop");
	TextDrawLetterSize(g_global_textdraws[6], 0.1809, 1.7160);
	TextDrawTextSize(g_global_textdraws[6], 520.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[6], 1);
	TextDrawColor(g_global_textdraws[6], -1);
	TextDrawUseBox(g_global_textdraws[6], 1);
	TextDrawBoxColor(g_global_textdraws[6], 0);
	TextDrawSetOutline(g_global_textdraws[6], 1);
	TextDrawBackgroundColor(g_global_textdraws[6], 255);
	TextDrawFont(g_global_textdraws[6], 1);
	TextDrawSetProportional(g_global_textdraws[6], 1);
	TextDrawSetShadow(g_global_textdraws[6], 0);
	TextDrawSetSelectable(g_global_textdraws[6], true);

	g_global_textdraws[7] = TextDrawCreate(521.7661, 368.0668, "info");
	TextDrawLetterSize(g_global_textdraws[7], 0.1809, 1.7160);
	TextDrawTextSize(g_global_textdraws[7], 533.0000, 0.0000);
	TextDrawAlignment(g_global_textdraws[7], 1);
	TextDrawColor(g_global_textdraws[7], -1);
	TextDrawUseBox(g_global_textdraws[7], 1);
	TextDrawBoxColor(g_global_textdraws[7], 0);
	TextDrawSetOutline(g_global_textdraws[7], 1);
	TextDrawBackgroundColor(g_global_textdraws[7], 255);
	TextDrawFont(g_global_textdraws[7], 1);
	TextDrawSetProportional(g_global_textdraws[7], 1);
	TextDrawSetShadow(g_global_textdraws[7], 0);
	TextDrawSetSelectable(g_global_textdraws[7], true);

	return true;
}

stock CreatePlayerTextDrawsInventory(playerid) {
	g_player_textdraw_slots[playerid][0] = CreatePlayerTextDraw(playerid, 456.9999, 116.4222, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][0], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][0], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][0], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][0], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][0], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][0], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][0], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][0], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][1] = CreatePlayerTextDraw(playerid, 496.2337, 116.4222, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][1], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][1], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][1], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][1], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][1], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][1], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][1], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][1], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][2] = CreatePlayerTextDraw(playerid, 536.4340, 116.4222, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][2], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][2], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][2], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][2], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][2], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][2], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][2], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][2], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][3] = CreatePlayerTextDraw(playerid, 576.8673, 116.4222, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][3], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][3], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][3], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][3], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][3], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][3], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][3], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][3], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][4] = CreatePlayerTextDraw(playerid, 456.8674, 156.8186, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][4], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][4], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][4], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][4], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][4], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][4], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][4], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][4], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][5] = CreatePlayerTextDraw(playerid, 496.0342, 156.8186, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][5], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][5], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][5], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][5], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][5], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][5], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][5], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][5], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][6] = CreatePlayerTextDraw(playerid, 536.3674, 156.8186, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][6], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][6], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][6], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][6], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][6], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][6], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][6], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][6], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][6], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][7] = CreatePlayerTextDraw(playerid, 576.7011, 156.8186, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][7], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][7], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][7], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][7], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][7], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][7], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][7], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][7], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][7], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][7], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][8] = CreatePlayerTextDraw(playerid, 456.7011, 197.1703, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][8], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][8], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][8], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][8], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][8], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][8], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][8], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][8], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][8], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][9] = CreatePlayerTextDraw(playerid, 496.0343, 197.1703, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][9], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][9], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][9], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][9], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][9], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][9], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][9], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][9], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][9], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][10] = CreatePlayerTextDraw(playerid, 536.3676, 196.8999, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][10], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][10], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][10], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][10], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][10], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][10], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][10], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][10], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][10], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][11] = CreatePlayerTextDraw(playerid, 576.7343, 197.0146, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][11], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][11], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][11], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][11], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][11], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][11], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][11], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][11], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][11], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][11], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][12] = CreatePlayerTextDraw(playerid, 456.7344, 237.4553, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][12], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][12], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][12], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][12], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][12], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][12], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][12], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][12], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][12], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][12], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][13] = CreatePlayerTextDraw(playerid, 496.3009, 237.5700, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][13], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][13], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][13], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][13], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][13], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][13], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][13], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][13], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][13], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][14] = CreatePlayerTextDraw(playerid, 536.4345, 237.7292, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][14], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][14], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][14], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][14], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][14], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][14], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][14], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][14], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][14], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][14], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][15] = CreatePlayerTextDraw(playerid, 576.7014, 237.8439, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][15], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][15], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][15], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][15], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][15], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][15], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][15], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][15], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][15], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][15], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][16] = CreatePlayerTextDraw(playerid, 456.7344, 278.5809, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][16], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][16], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][16], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][16], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][16], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][16], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][16], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][16], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][16], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][16], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][17] = CreatePlayerTextDraw(playerid, 496.2677, 278.5661, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][17], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][17], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][17], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][17], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][17], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][17], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][17], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][17], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][17], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][17], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][18] = CreatePlayerTextDraw(playerid, 536.3347, 278.5957, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][18], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][18], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][18], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][18], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][18], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][18], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][18], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][18], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][18], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][18], 0.0000, 0.0000, 0.0000, 1.0000);

	g_player_textdraw_slots[playerid][19] = CreatePlayerTextDraw(playerid, 576.8248, 278.5957, ""); // пусто
	PlayerTextDrawTextSize(playerid, g_player_textdraw_slots[playerid][19], 37.0000, 38.0000);
	PlayerTextDrawAlignment(playerid, g_player_textdraw_slots[playerid][19], 1);
	PlayerTextDrawColor(playerid, g_player_textdraw_slots[playerid][19], -1);
	PlayerTextDrawBackgroundColor(playerid, g_player_textdraw_slots[playerid][19], 100);
	PlayerTextDrawFont(playerid, g_player_textdraw_slots[playerid][19], 5);
	PlayerTextDrawSetProportional(playerid, g_player_textdraw_slots[playerid][19], 0);
	PlayerTextDrawSetShadow(playerid, g_player_textdraw_slots[playerid][19], 0);
	PlayerTextDrawSetSelectable(playerid, g_player_textdraw_slots[playerid][19], true);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][19], 19374);
	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][19], 0.0000, 0.0000, 0.0000, 1.0000);
	return true;
}

stock GetPlayerInventoryPage(playerid) {
	return g_player_page[playerid];
}

stock GetPlayerClickedSlot(playerid) {
	return g_player_clicked_slot[playerid];
}

stock HideInventoryTextDraws(playerid) {
	return true;
}

stock populateInventoryWithItems(playerid, inventory_page = 0) {
	foreach (new i: IterInventory[playerid]) {
		new
			item_id = GetInventoryData(playerid, inventory_page, i, ITEM_ID),
			item_slot = GetInventoryData(playerid, inventory_page, i, ITEM_SLOT),
			item_model = GetItemData(item_id, ITEM_MODEL)
		;

		PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][item_slot], item_model);
		PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][item_slot], 0.0, 0.0, 0.0, 5.0);
	}

	for (new i; i < sizeof g_player_textdraw_slots[]; i++) {
		PlayerTextDrawShow(playerid, g_player_textdraw_slots[playerid][i]);
	}

	for (new i; i < sizeof g_global_textdraws; i++) {
		TextDrawShowForPlayer(playerid, g_global_textdraws[i]);
	}

	return true;
}

stock ReloadSlot(playerid, slot, inventory_page = 0) {
	PlayerTextDrawHide(playerid, g_player_textdraw_slots[playerid][slot]);

	new item_id = GetInventoryData(playerid, inventory_page, slot, ITEM_ID);

	PlayerTextDrawSetPreviewRot(playerid, g_player_textdraw_slots[playerid][slot], 
		GetItemData(item_id, ITEM_DISPOSITION_X), GetItemData(item_id, ITEM_DISPOSITION_Y), GetItemData(item_id, ITEM_DISPOSITION_Z), GetItemData(item_id, ITEM_DISPOSITION_R)
	);
	PlayerTextDrawSetPreviewModel(playerid, g_player_textdraw_slots[playerid][slot], 
		GetItemData(item_id, ITEM_MODEL)
	);

	return PlayerTextDrawShow(playerid, g_player_textdraw_slots[playerid][slot]);
}

/*
	Commands
*/

cmd:add_item(playerid, params[]) {
	new page, slot, count, id;
	if (sscanf(params, "dddd", page, slot, count, id))
		return SendClientMessage(playerid, -1, " /add_item <page> <slot> <count <item_id>");

	if ((id < 1 || id > sizeof items_data))
		return SendClientMessage(playerid, -1, " Укажите валидный предмет.");

	if ((page < 0 || page > MAX_INVENTORY_PAGE))
		return SendClientMessage(playerid, -1, " Укажите валидную страницу.");

	if ((count < 0 || count > MAX_ITEM_COUNT))
		return SendClientMessage(playerid, -1, " Укажите валидное количество предмета. От 0 до 50-и.");

	new free_index = Iter_Free(IterInventory[playerid]);
	if (free_index == ITER_NONE) return print("> Inventory full.");

	SetInventoryData(playerid, page, free_index, ITEM_SLOT, free_index);
	SetInventoryData(playerid, page, free_index, ITEM_COUNT, count);
	SetInventoryData(playerid, page, free_index, ITEM_ID, id);

	Iter_Add(IterInventory[playerid], free_index);
	return SendClientMessage(playerid, -1, "Success");
}

cmd:inventory(playerid, params[]) {
	new inventory_page = 0;
	if (sscanf(params, "d", inventory_page)) 
		return SendClientMessage(playerid, -1, " /inventory <страница>");

	if ((inventory_page < 0 || inventory_page > MAX_INVENTORY_PAGE))
		return SendClientMessage(playerid, -1, " Укажите валидную страницу.");
	
	if (GetPlayerInventoryPage(playerid) == inventory_page)
		return SendClientMessage(playerid, -1, "Вы уже находитесь на этой странице.");

	SelectTextDraw(playerid, 0x1);
	is_open_inventory[playerid] = true;
	g_player_page[playerid] = inventory_page;

	return populateInventoryWithItems(playerid, inventory_page);
}

cmd:inventory_hide(playerid) {
	CancelSelectTextDraw(playerid);
	is_open_inventory[playerid] = false;
	return HideInventoryTextDraws(playerid);
}

/* Hooks */

hook OnGameModeInit() {
	CreateGlobalInventoryTextDraws();
	Iter_Init(IterInventory);
	return true;
}

hook OnPlayerConnect(playerid) {
	CreatePlayerTextDrawsInventory(playerid);
	return true;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText: playertextid) {
	if (IsOpenInventory(playerid)) {
 		for (new i; i < INVENTORY_SIZE; i++) {
			if (playertextid != g_player_textdraw_slots[playerid][i]) break;
			
			new
				inventory_page = GetPlayerInventoryPage(playerid),
				clickedslot = GetPlayerClickedSlot(playerid),
				rewrite_count = GetInventoryData(playerid, inventory_page, i, ITEM_COUNT),
				rewrite_slot = GetInventoryData(playerid, inventory_page, i, ITEM_SLOT),
				rewrite_id = GetInventoryData(playerid, inventory_page, i, ITEM_ID)
			;

			if (clickedslot == INVALID_INVENTORY_CLICK_SLOT && !rewrite_id) {
				break;
			}

			if (clickedslot == INVALID_INVENTORY_CLICK_SLOT) {
				g_player_clicked_slot[playerid] = i;
				break;
			}

			if (clickedslot != INVALID_INVENTORY_CLICK_SLOT && clickedslot != i) {
				new
					count = GetInventoryData(playerid, inventory_page, clickedslot, ITEM_COUNT),
					id = GetInventoryData(playerid, inventory_page, clickedslot, ITEM_ID),
					slot = GetInventoryData(playerid, inventory_page, clickedslot, ITEM_SLOT)
				;

				SetInventoryData(playerid, inventory_page, i, ITEM_COUNT, count);
				SetInventoryData(playerid, inventory_page, i, ITEM_ID, id);
				SetInventoryData(playerid, inventory_page, i, ITEM_SLOT, slot);

				SetInventoryData(playerid, inventory_page, clickedslot, ITEM_COUNT, rewrite_count);
				SetInventoryData(playerid, inventory_page, clickedslot, ITEM_ID, rewrite_id);
				SetInventoryData(playerid, inventory_page, clickedslot, ITEM_SLOT, rewrite_slot);

				ReloadSlot(playerid, i, inventory_page);
				ReloadSlot(playerid, clickedslot, inventory_page); 

				g_player_clicked_slot[playerid] = INVALID_INVENTORY_CLICK_SLOT;
				break;
			}
		}
	}
	
	return true;
}
