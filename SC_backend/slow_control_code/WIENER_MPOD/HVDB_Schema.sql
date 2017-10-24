CREATE TABLE Channel_values(
    hv_channel INTEGER, PRIMARY KEY     -- channel number on MPOD crate
    nominal_voltage DOUBLE,     -- nominal voltage from data sheet (V)
    current_limit DOUBLE,       -- current limit (A)
    voltage_ramp REAL,	  -- voltage ramp rate (V/s)
    enable BOOLEAN, -- is the channel enabled
    pmt_serial CHARACTER, -- PMT serial number
    daq_channel INTEGER, -- DAQ channel number
    entry_time TIME,  -- time of entry	
    array_id CHARACTER REFERENCES Gain_Points(pmt)		
);

CREATE TABLE Gains(
    pmt CHARACTER PRIMARY KEY
);

CREATE TABLE Gain_Points(
    array_id CHARACTER REFERENCES Gains(pmt),
    position INTEGER,      
    voltage REAL,
    gain REAL, 
    PRIMARY KEY (array_id, position)
)WITHOUT ROWID;
