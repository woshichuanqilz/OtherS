package nciic;

import java.io.BufferedReader;
import java.io.FileReader;
import java.lang.reflect.Proxy;
import java.security.Security;
import org.codehaus.xfire.client.Client;
import org.codehaus.xfire.client.XFireProxy;
import org.codehaus.xfire.client.XFireProxyFactory;
import org.codehaus.xfire.service.Service;
import org.codehaus.xfire.service.binding.ObjectServiceFactory;
import org.codehaus.xfire.util.dom.DOMOutHandler;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Proxy;
import java.net.MalformedURLException;
import org.apache.commons.httpclient.protocol.Protocol;
import org.apache.commons.httpclient.protocol.ProtocolSocketFactory;

import org.codehaus.xfire.service.binding.ObjectServiceFactory;
import org.codehaus.xfire.transport.http.EasySSLProtocolSocketFactory;
import org.codehaus.xfire.util.dom.DOMOutHandler;

public class IDCard {
	public static void main(String[] args) {
		if (args.length != 2) {
			System.out.println("请输入正确参数：身份证  姓名");
			System.exit(1);
			return ;
		}

		String IDCard = args[0];
		String Name = args[1];

		String SERVICE_URL = "https://ws.nciic.org.cn/nciic_ws/services/NciicServices";
		String serviceName = "";

		String result = null;
		try {

			System.setProperty("javax.net.ssl.trustStore", "c://nciic.keystore");
			System.setProperty("java.protocol.handler.pkgs", "com.sun.net.ssl.internal.www.protocol");
			Security.addProvider(new com.sun.net.ssl.internal.ssl.Provider());

			Service serviceModel = new ObjectServiceFactory().create(ServiceInf.class, "NciicServices", null, null);
			ServiceInf service = (ServiceInf) new XFireProxyFactory().create(serviceModel, SERVICE_URL + serviceName);
			Client client = ((XFireProxy) Proxy.getInvocationHandler(service)).getClient();
			client.addOutHandler(new DOMOutHandler());

			BufferedReader in = new BufferedReader(new FileReader("c://licensecode.txt"));
			String licensecode = in.readLine();
			String condition = GetXml(IDCard, Name);

			result = service.nciicCheck(licensecode, condition);
			if (result.contains("一致")) {
				if (result.contains("不一致")) {
					System.exit(1);
					return ;
				} else {
					System.exit(0);
					return ;
				}
				
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		System.exit(1);
	}

	public static String GetXml(String IDCard, String Name) {
		String Xml = "<?xml version='1.0' encoding='utf-8'?>" + "<ROWS>" + "<INFO>" + "<SBM>游酷盛世科技（北京）有限公司</SBM>"
				+ "</INFO>" + "<ROW>" + "<GMSFHM>公民身份号码</GMSFHM>" + "<XM>姓名</XM>" + "</ROW>"
				+ "<ROW FSD='10060' YWLX='防沉迷系统'>" + "<GMSFHM>" + IDCard + "</GMSFHM>" + "<XM>" + Name + "</XM>"
				+ "</ROW>" + "</ROWS>";

		return Xml;
	}

}
