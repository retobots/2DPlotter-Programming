Switch(mode)
{
    case: SD;
        processVector(AutoMode_Rect, 0, 0, w, h);
    case: Serial;
    case: IOT;
    case: Rectangle;
        processRect(AutoMode_Rect, 0, 0, w, h);

    case: Circle;
        processCircle(AutoMode_Rect, 0, 0, w, h);
}   