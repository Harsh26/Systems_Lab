					HOW TO RUN THE  TRADING SYSTEM				
					
		For running the trading system, you need to follow following Steps:-
		
					1. Make sure all the files zipped in the submission [server.c, client.c, login.txt] are in same folder. If there is 
						any file missing, the program might not run.
					
					2. Open terminal in the same directory, or traverse via 'cd' command to the directory where files are present.
					
					3. First of all, run the server, so that it will handel all the clients. Use following command:-
														
															> gcc server.c -o server
															> ./server <port_no>
															
					   Once the server is in listening phase, all the client can access the server.
					   
					4. Open another terminal as client in the same directory. Use following commands for the same:-
					
															> gcc client.c -o client
															> ./client <ip_address> <port_no>
															
																  
					5. Each Trader has a login ID and password. Login using TraderID and Password that is present in 'login.txt' file zipped 						in submission. Once login is successful, Follow instructions on screen to trade. 
