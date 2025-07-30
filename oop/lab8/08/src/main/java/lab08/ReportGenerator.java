package lab08;

import net.sf.jasperreports.engine.*;
import net.sf.jasperreports.engine.data.JRXmlDataSource;

import java.io.File;
import java.util.HashMap;

public class ReportGenerator {
    public void generateHtmlReport(String reportPath, String xmlFilePath, String outputFilePath) throws JRException {
        File reportFile = new File(reportPath);
        if (!reportFile.exists()) {
            throw new RuntimeException("Report file not found: " + reportPath);
        }

        File xmlFile = new File(xmlFilePath);
        if (!xmlFile.exists()) {
            throw new RuntimeException("XML data file not found: " + xmlFilePath);
        }

        JRXmlDataSource xmlDataSource = new JRXmlDataSource(xmlFile, "/Guests/Guest");

        JasperReport jasperReport = JasperCompileManager.compileReport(reportPath);
        HashMap<String, Object> parameters = new HashMap<>();
        JasperPrint jasperPrint = JasperFillManager.fillReport(jasperReport, parameters, xmlDataSource);

        JasperExportManager.exportReportToHtmlFile(jasperPrint, outputFilePath);
    }
}


