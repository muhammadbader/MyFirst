import sqlite3    #importing the sqlite3 library will result in a sqlite3 variable
import sys

dbcon = sqlite3.connect('moncafe.db') #create the connection
cursor = dbcon.cursor()

def addSupp(product_id, quantity,sup_id,date):
    cursor.execute("INSERT INTO Activities VALUES(?,?,?,?)", [product_id, quantity, sup_id, date])
    cursor.execute("""SELECT quantity FROM Products WHERE id=%s"""%(product_id))  # try it
    q = cursor.fetchone()
    quan = int(q[0])
    # where=(quan + quantity,product_id)
    # update="UPDATE Products SET quantity= %s WHERE id= %s"
    cursor.execute("UPDATE Products SET quantity= %s WHERE id= %s" %(quan + quantity,product_id))
    dbcon.commit()

def addEmpl(product_id, quantity, empl_id, date):

    cursor.execute("""SELECT quantity FROM Products WHERE id=%s"""% (product_id))  # try it    q=cursor.fetchone()
    q = cursor.fetchone()
    quan=int(q[0])
    if quan+quantity>=0:
        cursor.execute("INSERT INTO Activities VALUES(?,?,?,?)", [product_id, quantity, empl_id, date])
        cursor.execute("UPDATE Products SET quantity= %s WHERE id= %s" % (quan + quantity, product_id))
        dbcon.commit()

def main():
    f=open(sys.argv[1],"r")
    for w in f:
        x = w.split(", ")
        x[-1] = x[-1].strip()
        if int(x[1])<0:
            addEmpl(int(x[0]),int(x[1]),int(x[2]),int(x[3]))
        elif int(x[1])>0:
            addSupp(int(x[0]), int(x[1]), int(x[2]), int(x[3]))

main()
import printdb