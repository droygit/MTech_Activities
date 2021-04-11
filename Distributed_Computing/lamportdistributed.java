import java.util.*;
import java.io.*;
import java.net.*;

public class lamportdistributed{
	public static boolean search(LinkedList<Response> responses, Response r){
		for(Response x: responses){
			if(x.getNodeId() == r.getNodeId() && x.getTimeStamp() == r.getTimeStamp())
				return true;
		}
		return false;
	}
	public static void remove(LinkedList<Request> requests, Request r){
		Iterator<Request> iterator = requests.iterator();
		while(iterator.hasNext()){
			Request x= iterator.next();
			if(x.getNodeId() == r.getNodeId() && x.getTimeStamp() <= r.getTimeStamp()){
				iterator.remove();
			}
		}
	}
	public static int PORTS[] = {1000, 2000, 3000, 4000, 5000};
	public static int PORT;
	public static final int REQUEST  = 0;
	public static final int RESPONSE = 1;
	public static final int RELEASE = 2;
	public static void main(String[] args) throws Exception{
		int nodes = 2;
		boolean flag;
		byte buffer[] = new byte[1024];
		Scanner scan = new Scanner(System.in);
		LinkedList<Request> allRequests = new LinkedList<Request>();
		LinkedList<Request> requests = new LinkedList<Request>();
		LinkedList<Response> responses = new LinkedList<Response>();
		PORT = Integer.parseInt(args[0]);
		DatagramSocket ds = new DatagramSocket(PORT);
		for(int i = 1; i < args.length; i++){
			Request r = new Request(PORT, Integer.parseInt(args[i]));
			allRequests.add(r);
		}
		int timestamp = 1;
		Thread.sleep(1000);
		while(true){
			if(timestamp > 10)
				break;
			DatagramPacket p = new DatagramPacket(buffer, 1024);
			ds.setSoTimeout(10);
			flag = false;
			while(true){
				try{
					ds.receive(p);
					String s = new String(p.getData(), 0, p.getLength());
					scan = new Scanner(s);
					int type = scan.nextInt();
					int node = scan.nextInt();
					int t = scan.nextInt();
					switch(type){
						case REQUEST: requests.add(new Request(node, t));
						String result = String.valueOf(RESPONSE) + " " + String.valueOf(PORT) + " " + String.valueOf(timestamp);
						byte buf[] = result.getBytes();
						ds.send(new DatagramPacket(buf, buf.length, InetAddress.getLocalHost(), node)); 
							break;
						case RESPONSE: responses.add(new Response(node, t)); break;
						case RELEASE: remove(requests, new Request(node, t)); break;
							
					}
				}catch(SocketTimeoutException e){
					break;
				}
			}
			Request x;
			if(allRequests.peek() != null){
				x = allRequests.peek();
				if(x.getTimeStamp() <= timestamp){
					requests.add(new Request(PORT, timestamp));
					for(int i: PORTS){
						if(i == PORT)
							continue;
						String result = String.valueOf(REQUEST) + " " + String.valueOf(PORT) + " " + String.valueOf(timestamp);
						byte buf[] = result.getBytes();
						ds.send(new DatagramPacket(buf, buf.length, InetAddress.getLocalHost(), i));
					}
					allRequests.remove(x);
				}
			}
			if(requests.size() != 0){
				x = requests.get(0);
			if(x.getNodeId() == PORT){
				flag = true;
				for(int i = 0; i < nodes; i++){
					if(PORTS[i] == PORT)
						continue;
					int j;
					for(j = x.getTimeStamp(); j <= timestamp; j++)
					if(search(responses, new Response(PORTS[i], j)))
						break;
					if(j > timestamp)
						flag = false;
				}
				if(flag){
					System.out.println("Node " + PORT + " executing critical section at timestamp " + timestamp + "...");

					
					remove(requests, x);
					for(int i: PORTS){
						if(i == PORT)
							continue;
						String result = String.valueOf(RELEASE) + " " + String.valueOf(PORT) + " " + String.valueOf(timestamp);
						byte buf[] = result.getBytes();
						ds.send(new DatagramPacket(buf, buf.length, InetAddress.getLocalHost(), i));
					}
				}
			}
			
		}	
			timestamp++;
			Thread.sleep(1000);
	}
	}
}
class Request{
	private int timestamp;
	private int nodeId;
	public Request(int nodeId, int timestamp){
		this.timestamp = timestamp;
		this.nodeId = nodeId;
	}
	public int getNodeId(){
		return nodeId;
	}
	public int getTimeStamp(){
		return timestamp;
	}
}
class Response extends Request{
	public Response(int nodeId, int timestamp){
		super(nodeId, timestamp);
	}
}
