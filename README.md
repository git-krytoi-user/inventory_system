ИНСТРУКЦИЯ ПО ПОДКЛЮЧЕНИЮ
---------

Файл inventory_system.inc, необходимо поместить в /корень_сервера/pawno/include.
После того, как файл Вы поместили по расположению этой директории, 
Вам необходимо подключить его в моде, но перед этим, обязательно подключить дополнительные библиотеки, а именно: foreach (для системы это не обязательно, но желательно чтобы было подключено), Pawn.CMD, mdialog, a_mysql (R40 or higher), rustext (необходимо для руссификации текста, подробнее: https://pawn.wiki/index.php?/topic/35065-rustext/).

После всех подключённых библиотек, подключайте inventory_system.inc

РАБОТА С МОДОМ!
---------

до OnGameModeExit, вставить метод:

stock GetPlayerAccountID(playerid) {
	return PI[playerid][pID]; // PlayerInfo[playerid][pMysqlID];
}

В метод OnGameModeInit вставить следующий код:
mysql_pquery(connects, "CREATE TABLE IF NOT EXISTS `inventory` ( \n\
		`id` int(11) NOT NULL AUTO_INCREMENT, \n\
		`account_id` int(11) NOT NULL, \n\
		`item_id` int(4) NOT NULL, \n\
		`item_amount` int(2) NOT NULL, \n\
		`item_slot` int(2) NOT NULL, \n\
		`page` int(1) NOT NULL, \n\
		PRIMARY KEY (`id`))"
);

В метод OnPlayerConnect (в конец метода) и OnPlayerDisconnect (в конец метода) вставить код:
Inventory_CreatePlayerTextDraw(playerid);
new acc_id = GetPlayerAccountID(playerid);
SaveInventoryData(playerid, acc_id);

Вставить следующий код в метод CB: load_account(playerid)
new sql_query[512];
format(sql_query, sizeof sql_query, "SELECT * FROM inventory WHERE account_id = '%d' LIMIT 63", 
    PI[playerid][pID]
);
mysql_pquery(MySQL: 1, sql_query, "OnLoadInventory", "d", playerid);
sql_query[0] = '\0';
	
ptd_inventory[playerid][PLAYER_SKIN] = CreatePlayerTextDraw(playerid, 494.5664, 114.9108, "");
PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][PLAYER_SKIN], 106.0000, 104.0000);
PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][PLAYER_SKIN], 1);
PlayerTextDrawColor(playerid, ptd_inventory[playerid][PLAYER_SKIN], -1);
PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0);
PlayerTextDrawFont(playerid, ptd_inventory[playerid][PLAYER_SKIN], 5);
PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0);
PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0);
PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][PLAYER_SKIN], PI[playerid][pSkin]);
PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0.0000, 0.0000, 180.0000, 1.0000);

Немного воды, не совсем вода, но советую почитать!
--------
Разработчик данной системы, категорически не советует изменять такие значения как, INVENTORY_SIZE, INVENTORY_WIGHT, MAX_ITEMS (исключение, если добавляйте какой-либо предмет), MIN_PAGE, MAX_PAGE, и всё, что связано с лимитами инвентаря.
Объясняю почему. Лимиты - это самое важное, где угодно, и если их убрать - то мы сможем обезграничить память сервера, некоторыми лимитами, мы выделяем память, кстати, это так и есть!
Ещё один факт, почему не стоит изменять лимиты системы инвентаря, лимит - является выделением, например, размер инвентаря, к примеру, в этой системе инвентаря, макс. кол-во слотов = 16, соответственно, если Вы отнимите 1 от этой цифры, то получится так, что какой-то слот не будет работать.

Приятного пользования!
-----------
