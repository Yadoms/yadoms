# highcharts-export-clientside
Module for [HighCharts](http://www.highcharts.com/) to export charts client-side.

Webpage for this project is here: http://a----.github.io/highcharts-export-clientside/

You may need to export a chart you made using HighCharts to an image or a PDF. It has an exporting module but it relies on an export server, which by default is http://export.highcharts.com/ and you also –unlucky you– have one or more of the following:
* your app doesn't have access to the intertubes;
* your chart contains sensitive data and you don't want an unsecure channel to carry it;
* sensitive data or not, you don't trust HighCharts with it;
* it's against your company policies;
* you don't want to set an export server up;
* you think it's 2015 and FFS, _browsers should be able to do that_.

Additionally, it provides a common interface between the official export module and `export-csv`.

## Documentation

Want to give it a try in your project? Check its [Website](http://a----.github.io/highcharts-export-clientside/)
for installation instructions.

## Contribution

First, fork the project and clone it. Then, since dependencies are not shipped, so you will have to do the following:

```(sh)
bower install
```

Check the ```example.html``` file and mess with it. Once you are done, please consider making a pull request.

## Dependencies

This module depends on:
* [HighCharts](http://www.highcharts.com/) obviously, remember guys, it isn't free for commercial usages;
* its exporting module, that is bundle with it;
* for rasterized images (PNG, JPEG), a module called `canvas-tools` with is based<sup>1</sup> on [canvg](https://github.com/gabelerner/canvg) licenced under MIT Licence;
* [jsPDF](https://parall.ax/products/jspdf) (its GitHub page is [overthere](https://github.com/MrRio/jsPDF)) for PDF support, licenced under MIT Licence;
* Pseudo-official [export-csv](https://github.com/highslide-software/export-csv/tree/master) module for CSV and XLS support, under MIT Licence.

The only dependencies you must use are HighCharts and HighCharts exporting module. If you want PNG/JPEG, add `canvas-tools`. If you want PDF support, add both `canvas-tools` and `jsPDF`. If a dependency is missing for a file type, the option will not be available in the export menu.

<sup>1</sup> There are issues with canvg, title/subtitle appearing twice, this kind of things which `canvas-tools` fixes. So I'd suggest to go with this one.

## Changelog

### v1.1.4 – 2015-11-20

* New option to add BOM to CSV/XLS files. This fixes an issue in Microsoft Excel where file encoding sometimes where not recognized properly (#20)

### v1.1.3 – 2015-10-02

* Fixing an issue where the exported image would be cropped when both `scale` and `sourceWidth`/`sourceHeight` have been specified (#16)

### v1.1.2 – 2015-07-08

* Fixing issue in provided helper function for `exporting.filename` configuration option (#7)
* Wider range of dependencies versions in `bower.json` (#6)

### v1.1.1 – 2015-07-07

* Allow functions for the `exporting.filename` configuration option (#5)

### v1.1.0 – 2015-07-07

Should have been done months ago, sorry.
IE9 and below will not be supported in a near future and possibly any kind of future. Look at #4 for discussion on this matter.
* Async rendering, fixes issues when chart includes resources (icons and the likes, #1)
* Documentation fixes (#2)

### v1.0.1 – 2015-04-27

* handling of URL options in export-csv options (highslide-software/export-csv#40)

## Credits

Written with the help of the following resources:
* that  [Stack Overflow Q/A](http://stackoverflow.com/questions/25630811/export-highcharts-to-pdf-using-javascript-and-local-server-no-internet-connec) and this [snippet/project](https://github.com/leighquince/HighChartLocalExport/) by @leighquince;
* this [JSFiddle](http://jsfiddle.net/gh/get/jquery/1.7.2/highslide-software/highcharts.com/tree/master/samples/highcharts/exporting/offline-download/) from the official [HighCharts documentation](http://www.highcharts.com/docs/export-module/export-module-overview)
