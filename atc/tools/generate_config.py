import os
import datetime 
import socket
import sys

def generate_comment(comment):
    return '# ' + str(comment) + '\n'

def generate_row(key,val):
    return str(key) + ': ' + str(val) + '\n'

def main():
    output = ""

    #ouput time of generation
    output += generate_comment("Created on " + str(datetime.datetime.now()) + "\n")

    #prompt for name of drone
    sys.stderr.write("Enter drone name: ")
    drone_name = raw_input()
    output += generate_row('drone_name',drone_name)

    #find our ip address
    hostname = socket.gethostname()
    output += generate_row('drone_ip',socket.gethostbyname(hostname))
    
    #resolve atc ip address
    #DEBUG: output += generate_row('atc_ip','10.0.0.2')
    #PRODUCTION:    
    output += generate_row('atc_ip',socket.gethostbyname_ex('ascenddeliveries.duckdns.org')[2][0])

    print('localwhatever',socket.getfqdn())

    #print out config file
    print(output)



if __name__ == "__main__":
    main()