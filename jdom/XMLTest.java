import org.jdom.*;
import org.jdom.input.*;
import org.jdom.output.*;

class XMLTest
{
    public static void main(String[] args) throws Exception {
        SAXBuilder builder = new SAXBuilder();
        Document doc = builder.build("test.xml");
        Element root = doc.getRootElement();

        Element h = new Element("Collection");
        root.addContent( h );

        XMLOutputter outp = new XMLOutputter();

        outp.output(doc, System.out);

        String xmlResult = outp.outputString(doc);
        System.out.println("String result");
        System.out.println(xmlResult);
    } // end - public static void main(String[] args)

} // end - class XMLTest

