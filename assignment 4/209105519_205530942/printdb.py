import sqlite3    #importing the sqlite3 library will result in a sqlite3 variable

dbcon = sqlite3.connect('moncafe.db') #create the connection
cursor = dbcon.cursor()

def main():
    print ("Activities")
    print_activities
    print("Coffee stands")
    print_table("Coffee_stands")
    print("Employees")
    print_table("Employees")
    print("Products")
    print_table("Products")
    print("Suppliers")
    print_table("Suppliers")
    print()
    print("Employees report")
    employeeReport()

    detailer_activities()


def print_table(table_name):
    cursor.execute('SELECT * FROM ' + table_name + ' ORDER BY id ASC')
    list = cursor.fetchall()
    for item in list:
        print(item)

def print_activities():
    cursor.execute('SELECT * FROM Activities ORDER BY date ASC')
    list = cursor.fetchall()
    for item in list:
        print(item)

def employeeReport():
    cursor.execute("SELECT empl.name, empl.salary, coff.location FROM Employees empl "
                   "JOIN Coffee_stands coff ON empl.coffee_stand = coff.id")
    list=cursor.fetchall()
    cursor.execute("""SELECT empl.name,act.quantity,pro.price FROM Activities act
                      JOIN Products pro ON act.Product_id=pro.id 
                      Join Employees empl ON act.activator_id=empl.id""")
    list2=cursor.fetchall()
    totalsale=[]
    names=[]
    for item in list2:
        if float(item[1])<0:
            new=True
            i=0
            for s in names:
                if s==item[0]:
                    totalsale[i]=totalsale[i]+(-1 * float(item[1]) * float(item[2]))
                    new=False
                    break
                else:
                    i+=1
            if new:
                totalsale.append(-1 * float(item[1]) * float(item[2]))
                names.append(str(item[0]))


    for item in list:
        i=0
        there=True
        for name in names:
            if name==str(item[0]):
                print(str(item[0] + " " + str(item[1]) + " " + str(item[2])+" "+str(totalsale[i])))
                there=False
                break
            else:
                i += 1
        if there:
            print(str(item[0]) + " " + str(item[1]) + " " + str(item[2]) + " 0")

def detailer_activities():
     cursor.execute("""SELECT act.date, pro.description, act.quantity,empl.name,supp.name FROM Activities act
                       JOIN Products pro ON act.Product_id=pro.id
                       LEFT JOIN Employees empl ON act.activator_id=empl.id
                       LEFT JOIN Suppliers supp ON act.activator_id=supp.id""")
     detailed=cursor.fetchall()
     if detailed:
         print()
         print ("Activities")
         for item in detailed:
             print(item)

main()
