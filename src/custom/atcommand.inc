// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Atcommands
 * Place the body of custom atcommands in this file.
 * Format:
 *	ACMD_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

ACMD_FUNC(ap)
{
	int32 ap = 0;
	nullpo_retr(-1, sd);

	if (!pc_is_trait_job(sd->class_)) {
		clif_displaymessage(fd, msg_txt(sd, 797)); // This command is unavailable to non-4th class.
		return -1;
	}

	sscanf(message, "%11d", &ap);

	// Overflow check.
	if (ap == INT_MIN) ap++;

	if (ap == 0) {
		if (!status_percent_heal(sd, 0, 0, 100))
			clif_displaymessage(fd, msg_txt(sd, 823)); // AP have already been recovered.
		else
			clif_displaymessage(fd, msg_txt(sd, 821)); // AP recovered.
		return 0;
	} else if (ap > 0) {
		if (!status_heal(sd, 0, 0, ap, 0))
			clif_displaymessage(fd, msg_txt(sd, 823)); // AP have already been recovered.
		else
			clif_displaymessage(fd, msg_txt(sd, 821)); // AP recovered.
		return 0;
	} else {
		status_damage(nullptr, sd, 0, 0, -ap, 0, 0, 0);
		clif_displaymessage(fd, msg_txt(sd, 822)); // AP modified.
		return 0;
	}
}
