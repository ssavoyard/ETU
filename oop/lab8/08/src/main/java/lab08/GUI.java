package lab08;

import org.w3c.dom.*;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import org.xml.sax.SAXException;

public class GUI {
    private JFrame frame;
    private JMenuBar menuBar;
    private JMenu fileMenu, sortMenu, exportMenu;
    private JMenuItem openItem, saveItem, roomItem, nameItem, multiThreadItem;
    private JToolBar toolBar;
    private JButton addButton, deleteButton, searchButton;
    private JComboBox<String> searchType;
    private JTextField searchField;
    private JTable dataTable;
    private JScrollPane tableScrollPane;
    private DefaultTableModel tableModel;

    private final Object monitor = new Object();
    private boolean isXmlGenerated = false;
    private boolean isDataLoaded = false;

    public void buildAndShowGUI() {
        frame = new JFrame("Hotel - Guest List");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 600);

        // Menu bar
        menuBar = new JMenuBar();
        fileMenu = new JMenu("File");
        openItem = new JMenuItem("Open");
        saveItem = new JMenuItem("Save");
        fileMenu.add(openItem);
        fileMenu.add(saveItem);
        menuBar.add(fileMenu);

        // Sort menu
        sortMenu = new JMenu("Sort by");
        roomItem = new JMenuItem("Room");
        nameItem = new JMenuItem("Name");
        sortMenu.add(nameItem);
        sortMenu.add(roomItem);
        menuBar.add(sortMenu);

        // Multithreading menu
        exportMenu = new JMenu("Export");
        multiThreadItem = new JMenuItem("Multithreading");
        exportMenu.add(multiThreadItem);
        menuBar.add(exportMenu);

        frame.setJMenuBar(menuBar);

        // Toolbar
        toolBar = new JToolBar();
        addButton = new JButton("Add");
        deleteButton = new JButton("Delete");
        toolBar.add(addButton);
        toolBar.add(deleteButton);
        frame.add(toolBar, BorderLayout.NORTH);

        // Search panel
        JPanel searchPanel = new JPanel();
        searchType = new JComboBox<>(new String[]{"Name", "Room", "Date"});
        searchField = new JTextField(15);
        searchButton = new JButton("Search");
        searchPanel.add(new JLabel("Search by:"));
        searchPanel.add(searchType);
        searchPanel.add(searchField);
        searchPanel.add(searchButton);
        frame.add(searchPanel, BorderLayout.SOUTH);

        // Table setup
        String[] columns = {"Name", "Room", "Check-in Date", "Check-out Date"};
        tableModel = new DefaultTableModel(columns, 0);
        dataTable = new JTable(tableModel);
        tableScrollPane = new JScrollPane(dataTable);
        frame.add(tableScrollPane, BorderLayout.CENTER);

        addListeners();
        frame.setVisible(true);
    }

    private void addListeners() {
        saveItem.addActionListener(e -> saveDataToFile());
        openItem.addActionListener(e -> loadDataFromFile());

        multiThreadItem.addActionListener(e -> startMultithreading(tableModel));

        addButton.addActionListener(e -> {
            String name = JOptionPane.showInputDialog("Enter guest name:");
            String room = JOptionPane.showInputDialog("Enter room number:");
            String checkIn = JOptionPane.showInputDialog("Enter check-in date (dd.mm.yyyy):");
            String checkOut = JOptionPane.showInputDialog("Enter check-out date (dd.mm.yyyy):");
            tableModel.addRow(new Object[]{name, room, checkIn, checkOut});
        });

        deleteButton.addActionListener(e -> {
            int selectedRow = dataTable.getSelectedRow();
            if (selectedRow != -1) {
                tableModel.removeRow(selectedRow);
            }
        });
    }

    private void startMultithreading(DefaultTableModel tableModel) {
        // Helper flags for synchronization
        isXmlGenerated = false;
        isDataLoaded = false;

        // First thread
        Thread dataLoader = new Thread(() -> {
            synchronized (monitor) {
                System.out.println("First thread: Data loaded");
                isDataLoaded = true;
                monitor.notifyAll();
            }
        });

        // Second thread
        Thread xmlEditor = new Thread(() -> {
            synchronized (monitor) {
                try {
                    while (!isDataLoaded) {
                        monitor.wait();
                    }
                    System.out.println("Second thread: Generating XML...");
                    XMLFile.saveToXML(tableModel, new File("src/docs/HotelReport.xml"));
                    System.out.println("Second thread: XML successfully saved!");
                    isXmlGenerated = true;
                    monitor.notifyAll();
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }
        });

        // Third thread (empty for now)
        Thread reportGenerator = new Thread(() -> {
            synchronized (monitor) {
                try {
                    while (!isXmlGenerated) {
                        monitor.wait();
                    }
                    System.out.println("Third thread: No operation defined.");
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
            }
        });

        dataLoader.start();
        xmlEditor.start();
        reportGenerator.start();
    }

    private void saveDataToFile() {
        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showSaveDialog(frame) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                Document doc = dBuilder.newDocument();

                // Root element
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

                JOptionPane.showMessageDialog(frame, "Data saved successfully!");
            } catch (ParserConfigurationException | TransformerException | IOException ex) {
                JOptionPane.showMessageDialog(frame, "Error saving file: " + ex.getMessage());
            }
        }
    }

    private void loadDataFromFile() {
        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showOpenDialog(frame) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                DocumentBuilder dBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
                Document doc = dBuilder.parse(file);
                doc.getDocumentElement().normalize();

                tableModel.setRowCount(0);

                NodeList guestList = doc.getElementsByTagName("Guest");
                for (int i = 0; i < guestList.getLength(); i++) {
                    Node guestNode = guestList.item(i);
                    if (guestNode.getNodeType() == Node.ELEMENT_NODE) {
                        NamedNodeMap attributes = guestNode.getAttributes();
                        String name = attributes.getNamedItem("Name").getNodeValue();
                        String room = attributes.getNamedItem("Room").getNodeValue();
                        String checkIn = attributes.getNamedItem("CheckIn").getNodeValue();
                        String checkOut = attributes.getNamedItem("CheckOut").getNodeValue();

                        tableModel.addRow(new String[]{name, room, checkIn, checkOut});
                    }
                }

                JOptionPane.showMessageDialog(frame, "File loaded successfully!");

            } catch (ParserConfigurationException | SAXException | IOException ex) {
                JOptionPane.showMessageDialog(frame, "Error opening file: " + ex.getMessage());
            }
        }
    }

    private void validateGuestInput(String name, String room, String checkIn, String checkOut) throws EmptyFieldException {
        if (name.isEmpty() || room.isEmpty() || checkIn.isEmpty() || checkOut.isEmpty()) {
            throw new EmptyFieldException("All fields are required!");
        }
    }
}

