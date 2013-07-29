
SUBDIRS = lib \
	alarm_alert_system \
	alarm_trip_system \
	ADAM6000 \
	Alicat \
	AMI286 \
	cams \
	CDL204 \
        disk_free \
 	DWP_scale \
	CryoCon18C \
        CryoCon24C \
	CryoCon34 \
	CryoCon44 \
        CryoMag_LM500 \
        DiffP \
        GFM373 \
	Hornet \
	Keithley485 \
	LeCroy1458 \
        LevelBox \
	LS218 \
	LS330 \
	LS330Keithley \
	LS331 \
	LUX_DAQ \
	MKS925C \
	NI_PCI_622X \
	ORTEC974 \
	PDR2000 \
	postproc \
        Remote_Watchdog \
	SC_Watchdog \
	SL130 \
        SPS125 \
	SRS350 \
	SRS_RGA \
	SuperLogics \
	TPG26X \
        TSctrl \
	TTi_PowerSupply \
	UTI \
	UTILM_RH \
	Watlow_EZ_TC \
        YLM001 \

.PHONY: $(SUBDIRS)


all clean install:
	for dir in $(SUBDIRS); do \
	  make -C $$dir $@; \
	done


