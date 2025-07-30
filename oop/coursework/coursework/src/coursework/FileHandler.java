package coursework;

import org.w3c.dom.*;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class FileHandler {

    public static void saveData(JFrame frame, DefaultTableModel roomModel, DefaultTableModel guestModel, DefaultTableModel employeeModel, DefaultTableModel priceModel) {
        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showSaveDialog(frame) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                Document doc = dBuilder.newDocument();

                // Root element
                Element rootElement = doc.createElement("HotelData");
                doc.appendChild(rootElement);

                // Rooms
                Element roomsElement = doc.createElement("Rooms");
                rootElement.appendChild(roomsElement);
                for (int i = 0; i < roomModel.getRowCount(); i++) {
                    Element roomElement = doc.createElement("Room");
                    roomElement.setAttribute("Number", roomModel.getValueAt(i, 0).toString());
                    roomElement.setAttribute("Type", roomModel.getValueAt(i, 1).toString());
                    roomElement.setAttribute("OccupiedBy", roomModel.getValueAt(i, 2).toString());
                    roomsElement.appendChild(roomElement);
                }

                // Guests
                Element guestsElement = doc.createElement("Guests");
                rootElement.appendChild(guestsElement);
                for (int i = 0; i < guestModel.getRowCount(); i++) {
                    Element guestElement = doc.createElement("Guest");
                    guestElement.setAttribute("Name", guestModel.getValueAt(i, 0).toString());
                    guestElement.setAttribute("RoomNumber", guestModel.getValueAt(i, 1).toString());
                    guestElement.setAttribute("StayDuration", guestModel.getValueAt(i, 2).toString());
                    guestsElement.appendChild(guestElement);
                }

                // Employees
                Element employeesElement = doc.createElement("Employees");
                rootElement.appendChild(employeesElement);
                for (int i = 0; i < employeeModel.getRowCount(); i++) {
                    Element employeeElement = doc.createElement("Employee");
                    employeeElement.setAttribute("Name", employeeModel.getValueAt(i, 0).toString());
                    employeeElement.setAttribute("Position", employeeModel.getValueAt(i, 1).toString());
                    employeeElement.setAttribute("RoomType", employeeModel.getValueAt(i, 2).toString());
                    employeesElement.appendChild(employeeElement);
                }

                // Prices
                Element pricesElement = doc.createElement("Prices");
                rootElement.appendChild(pricesElement);
                for (int i = 0; i < priceModel.getRowCount(); i++) {
                    Element priceElement = doc.createElement("Price");
                    priceElement.setAttribute("ServiceType", priceModel.getValueAt(i, 0).toString());
                    priceElement.setAttribute("Price", priceModel.getValueAt(i, 1).toString());
                    pricesElement.appendChild(priceElement);
                }

                // Write to file
                TransformerFactory transformerFactory = TransformerFactory.newInstance();
                Transformer transformer = transformerFactory.newTransformer();
                transformer.setOutputProperty(OutputKeys.INDENT, "yes");
                DOMSource source = new DOMSource(doc);
                StreamResult result = new StreamResult(new FileWriter(file));
                transformer.transform(source, result);

                JOptionPane.showMessageDialog(frame, "Data saved successfully!");

            } catch (ParserConfigurationException | TransformerException | IOException ex) {
                JOptionPane.showMessageDialog(frame, "Error saving file: " + ex.getMessage());
            }
        }
    }

    public static void loadData(JFrame frame, DefaultTableModel roomModel, DefaultTableModel guestModel, DefaultTableModel employeeModel, DefaultTableModel priceModel) {
        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showOpenDialog(frame) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                DocumentBuilder dBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
                Document doc = dBuilder.parse(file);
                doc.getDocumentElement().normalize();

                // Clear current data
                roomModel.setRowCount(0);
                guestModel.setRowCount(0);
                employeeModel.setRowCount(0);
                priceModel.setRowCount(0);

                // Rooms
                NodeList roomList = doc.getElementsByTagName("Room");
                for (int i = 0; i < roomList.getLength(); i++) {
                    Node node = roomList.item(i);
                    if (node.getNodeType() == Node.ELEMENT_NODE) {
                        Element element = (Element) node;
                        roomModel.addRow(new Object[]{
                                element.getAttribute("Number"),
                                element.getAttribute("Type"),
                                element.getAttribute("OccupiedBy")
                        });
                    }
                }

                // Guests
                NodeList guestList = doc.getElementsByTagName("Guest");
                for (int i = 0; i < guestList.getLength(); i++) {
                    Node node = guestList.item(i);
                    if (node.getNodeType() == Node.ELEMENT_NODE) {
                        Element element = (Element) node;
                        guestModel.addRow(new Object[]{
                                element.getAttribute("Name"),
                                element.getAttribute("RoomNumber"),
                                element.getAttribute("StayDuration")
                        });
                    }
                }

                // Employees
                NodeList employeeList = doc.getElementsByTagName("Employee");
                for (int i = 0; i < employeeList.getLength(); i++) {
                    Node node = employeeList.item(i);
                    if (node.getNodeType() == Node.ELEMENT_NODE) {
                        Element element = (Element) node;
                        employeeModel.addRow(new Object[]{
                                element.getAttribute("Name"),
                                element.getAttribute("Position"),
                                element.getAttribute("RoomType")
                        });
                    }
                }

                // Prices
                NodeList priceList = doc.getElementsByTagName("Price");
                for (int i = 0; i < priceList.getLength(); i++) {
                    Node node = priceList.item(i);
                    if (node.getNodeType() == Node.ELEMENT_NODE) {
                        Element element = (Element) node;
                        priceModel.addRow(new Object[]{
                                element.getAttribute("ServiceType"),
                                element.getAttribute("Price")
                        });
                    }
                }

                JOptionPane.showMessageDialog(frame, "File loaded successfully!");

            } catch (Exception ex) {
                JOptionPane.showMessageDialog(frame, "Error opening file: " + ex.getMessage());
            }
        }
    }
}
