
import sqlite3    #importing the sqlite3 library will result in a sqlite3 variable
import os         #Just for os.path.isfile(...
import atexit     #To define "at exit" behavior (Like finally in Java)
import sys

DBExist = os.path.isfile('moncafe.db')
if DBExist: #if the file exists beforehead remove it and create a new one
    os.remove('moncafe.db')

dbcon = sqlite3.connect('moncafe.db') #create the connection
cursor = dbcon.cursor()

def close_cafe():
    dbcon.commit()
    dbcon.close()
    # os.remove('moncafe.db')

atexit.register(close_cafe)

def create_cafe_tables():
        cursor.execute("""CREATE TABLE Employees(id     INTEGER PRIMARY KEY,
                                                 name   TEXT not null,
                                                 salary REAL NOT NULL,
                                                 coffee_stand  INTEGER NOT NULL,
                                                  FOREIGN KEY(coffee_stand)   REFERENCES Coffee_stands(id))
        """)
        cursor.execute("CREATE TABLE Suppliers(id   INTEGER PRIMARY KEY,"
                                              "name TEXT NOT NULL,"
                                               "contact_information TEXT"
                       ")")

        cursor.execute("CREATE TABLE Products(id   INTEGER PRIMARY KEY,"
                       "description TEXT NOT NULL,"
                       "price REAL NOT NULL,"
                       "quantity INTEGER NOT NULL"
                       ")")

        cursor.execute("CREATE TABLE Coffee_stands("
                       "id   INTEGER PRIMARY KEY,"
                       "location TEXT NOT NULL,"
                       "number_of_employees INTEGER"
                       ")")

        cursor.execute("CREATE TABLE Activities("
                       "product_id   INTEGER,"
                       "quantity INTEGER NOT NULL,"
                       "activator_id INTEGER NOT NULL,"
                       "date DATE NOT NULL,"
                       "FOREIGN KEY(product_id)   REFERENCES Products(id),"
                       "FOREIGN KEY(activator_id)   REFERENCES Employees(id),"
                       "FOREIGN KEY(activator_id)   REFERENCES Suppliers(id)"
                       ")")

def E(id,name,salary,coffee_stand):
    cursor.execute("INSERT INTO Employees VALUES(?,?,?,?)",[id,name,salary,coffee_stand])
def C(id,location,num_of_employees):
    cursor.execute("INSERT INTO Coffee_stands VALUES(?,?,?)", [id,location,num_of_employees])
def S(id,name,contact_info):
    cursor.execute("INSERT INTO Suppliers VALUES(?,?,?)", [id, name, contact_info])
def P(id,destination,price):
    cursor.execute("INSERT INTO Products VALUES(?,?,?,?)",[id,destination,price,0])

def main():
    create_cafe_tables()
    #todo check table keys!!!
    f=open(sys.argv[1],"r")
    for x in f:
        q = x.split(", ")
        q[-1] = q[-1].strip()
        if x[0]=="S":
            S(int(q[1]),q[2],q[3])
        elif x[0]=="E":
            E(int(q[1]),q[2],q[3],int(q[4]))
        elif x[0]=="C":
            C(int(q[1]), q[2], int(q[3]))
        elif x[0]=="P":
            P(int(q[1]), q[2], q[3])

    f.close()

main()
