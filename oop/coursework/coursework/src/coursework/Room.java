package coursework;

public class Room {
    private int roomNumber;
    private String roomType;
    private String occupiedBy;

    public Room(int roomNumber, String roomType, String occupiedBy) {
        this.roomNumber = roomNumber;
        this.roomType = roomType;
        this.occupiedBy = occupiedBy;
    }

    public int getRoomNumber() {
        return roomNumber;
    }

    public void setRoomNumber(int roomNumber) {
        this.roomNumber = roomNumber;
    }

    public String getRoomType() {
        return roomType;
    }

    public void setRoomType(String roomType) {
        this.roomType = roomType;
    }

    public String getOccupiedBy() {
        return occupiedBy;
    }

    public void setOccupiedBy(String occupiedBy) {
        this.occupiedBy = occupiedBy;
    }
}