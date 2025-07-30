package lab08;

import org.w3c.dom.*;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.swing.table.DefaultTableModel;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class XMLFile {
    public static void saveToXML(DefaultTableModel tableModel, File file) throws ParserConfigurationException, TransformerException, IOException {
        DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
        Document doc = dBuilder.newDocument();

        Element rootElement = doc.createElement("Guests");
        doc.appendChild(rootElement);

        for (int i = 0; i < tableModel.getRowCount(); i++) {
            Element guest = doc.createElement("Guest");
            guest.setAttribute("Name", (String) tableModel.getValueAt(i, 0));
            guest.setAttribute("Room", (String) tableModel.getValueAt(i, 1));
            guest.setAttribute("CheckIn", (String) tableModel.getValueAt(i, 2));
            guest.setAttribute("CheckOut", (String) tableModel.getValueAt(i, 3));
            rootElement.appendChild(guest);
        }

        TransformerFactory transformerFactory = TransformerFactory.newInstance();
        Transformer transformer = transformerFactory.newTransformer();
        transformer.setOutputProperty(OutputKeys.INDENT, "yes");
        DOMSource source = new DOMSource(doc);
        StreamResult result = new StreamResult(new FileWriter(file));
        transformer.transform(source, result);
    }
}
