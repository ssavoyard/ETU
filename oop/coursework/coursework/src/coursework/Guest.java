package coursework;

public class Guest {
    private String name;
    private int roomNumber;
    private int stayDuration;

    public Guest(String name, int roomNumber, int stayDuration) {
        this.name = name;
        this.roomNumber = roomNumber;
        this.stayDuration = stayDuration;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getRoomNumber() {
        return roomNumber;
    }

    public void setRoomNumber(int roomNumber) {
        this.roomNumber = roomNumber;
    }

    public int getStayDuration() {
        return stayDuration;
    }

    public void setStayDuration(int stayDuration) {
        this.stayDuration = stayDuration;
    }
}