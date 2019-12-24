#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int selectCallback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s:\n", (const char*) data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

static int updateCallback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s:\n", (const char*) data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int deleteCallback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s:\n", (const char*) data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char* argv[]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	bool databaseOpenFlag = false;
	bool tableInsertFlag = false;
	const char* insertData;
	const char* updateData;
	const char* deleteData;

	/* Open database */
	rc = sqlite3_open("test.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	} else {
		fprintf(stdout, "Opened database successfully\n");
		databaseOpenFlag = true;
	}

	if (databaseOpenFlag) {
		/* Create SQL statement */
		sql = "CREATE TABLE COMPANY("
				"ID INTEGER PRIMARY KEY AUTOINCREMENT,"
				"NAME           TEXT    NOT NULL,"
				"AGE            INT     NOT NULL,"
				"ADDRESS        CHAR(50),"
				"SALARY         REAL );";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		} else {
			fprintf(stdout, "Table created successfully\n");
		}

		/* Create SQL statement */
		sql = "INSERT INTO COMPANY (NAME,AGE,ADDRESS,SALARY) "
				"VALUES ('Paul', 32, 'California', 20000.00 ); "
				"INSERT INTO COMPANY (NAME,AGE,ADDRESS,SALARY) "
				"VALUES ('Allen', 25, 'Texas', 15000.00 ); "
				"INSERT INTO COMPANY (NAME,AGE,ADDRESS,SALARY)"
				"VALUES ('Teddy', 23, 'Norway', 20000.00 );"
				"INSERT INTO COMPANY (NAME,AGE,ADDRESS,SALARY)"
				"VALUES ('Mark', 25, 'Rich-Mond ', 65000.00 );";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		} else {
			fprintf(stdout, "Records created successfully\n");
			tableInsertFlag = true;
		}
	}

	if (tableInsertFlag) {
		/* Create SQL statement */
		updateData = "--- SELECT Callback function called ---";
		sql = "SELECT * from COMPANY";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, selectCallback, (void*) updateData,
				&zErrMsg);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		} else {
			fprintf(stdout, "Operation done successfully\n");
		}

		/* Create merged SQL statement */
		updateData = "--- UPDATE Callback function called ---";
		sql =
				"UPDATE COMPANY set 'SALARY' = 35000.00 where `ADDRESS`='California'; "
						"SELECT * from COMPANY";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, updateCallback, (void*) updateData,
				&zErrMsg);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		} else {
			fprintf(stdout, "Operation done successfully\n");
		}

		/* Create merged SQL statement */
		deleteData = "--- DELETE Callback function called ---";
		sql = "DELETE from COMPANY where `NAME`='Mark'; "
				"SELECT * from COMPANY";

		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, deleteCallback, (void*) deleteData,
				&zErrMsg);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		} else {
			fprintf(stdout, "Operation done successfully\n");
		}
	}
	sqlite3_close(db);
	system("pause");
	return 0;
}
