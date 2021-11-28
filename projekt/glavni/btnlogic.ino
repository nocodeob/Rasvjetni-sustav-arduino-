// normal button logic
void btnLogic ()
{
	if (debouncer3.fell() && screenPicker > 0)
	{
		screenPicker--;
		screenPickerTracker = false;
	}

	if (debouncer4.fell() && screenPicker < 4)
	{
		screenPicker++;
		screenPickerTracker = false;
	}
}
