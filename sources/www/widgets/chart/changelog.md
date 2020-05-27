### 1.4.0-beta.1
* new feature #536 - Navigate into data
* Fix #585 - tab "1h" : elements are no more updated continuously
* Fix continuous display for enum data
* new feature : tab "1h" : Don't connect points when no data during a time > 2x (mean time of the interval of the returned data)
* Better scale display

### 1.3.4
* Better keyword selection filtering

### 1.3.3
* Fix #521 - adapt the widget to display duration with the new format

### 1.3.2
* Fix Zoom for Y position
* Fix zooming and panning for PC

### 1.3.1
* Fix #554 - Curves reappears after changing the period

### 1.3.0
* Add the possibility to disable automatic unit scaling

### 1.2.2
* Fix unit and values adaptation with one axis
* Fix coefficient adaptation when multiple curves are displayed with an adaptation for each

### 1.2.1
* Fix #545 - No need legend y axis for boolean keyword

### 1.2.0
* Fix rare error due to highchart internal behaviour
* Fix #519 - Enum value are not displayed properly
* Fix the option same axis for all curves
* Fix #516 - Units inverted between 2 curves
* Fix #517 - A lot of errors and some curves missing
* Fix #508 - Error when displaying some charts
* Fix #515 - Boolean not well displayed
* NewFeature #466 - Adapt units if needed
* NewFeature #485 - Adapt unit for bit/s display
* Fix #389 - Bad display after computer wake up
* Fix #500 - error log every minutes for only few tabs
* Fix #503 - Export file - hour format in GMT hour
* Fix #505 - Mismatch export function when more than one chart in a page

### 1.1.0
* Fix #462 : doesn't display the first value for some summary values
* clean unused variable and elements
* Add the choice of different summary data (hourly, daily, weekly, monthly) of each period
* Add the possibility to display 5 years of data

### 1.0.3
* Add the possibility to display enum values