#include <vtkSmartPointer.h>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkElevationFilter.h>
#include <vtkProperty.h>
#include <vtkDeformPointSet.h>
#include <vtkCamera.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkOBJImporter.h>
#include <vtkOBJReader.h>
#include <vtkImageShiftScale.h>
#include <vtkBMPWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkOBJExporter.h>
#include <sstream>
#include <vtkJPEGReader.h>
#include <vtkImageData.h>
#include <vtkTexture.h>

//class vtkOBJPolyDataProcessor;

int main(int argc, char *argv[]) {
	vtkSmartPointer<vtkPolyData> input =
			vtkSmartPointer<vtkPolyData>::New();
	double bounds[6];

	vtkSmartPointer<vtkOBJReader> reader =
			vtkSmartPointer<vtkOBJReader>::New();

		std::string inputFilename = argv[1];

	reader->SetFileName(inputFilename.c_str());
	//reader->SetFileNameMTL("materials.mtl");

	vtkSmartPointer<vtkRenderer> renderer =
			vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin =
			vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> iren =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	//reader->SetRenderWindow(renWin);

	reader->Update();


//	reader->Impl->
		input->ShallowCopy(reader->GetOutput());
		input->GetBounds(bounds);
//input->ShallowCopy(renWin->Get)




	vtkSmartPointer<vtkOBJReader> reader2 =
			vtkSmartPointer<vtkOBJReader>::New();
	reader2->SetFileName("iamgroot101_surface.obj");
	reader2->Update();
	vtkSmartPointer<vtkPolyData> pd =
			vtkSmartPointer<vtkPolyData>::New();
	pd->ShallowCopy(reader2->GetOutput());
	vtkSmartPointer<vtkPoints> pts = pd->GetPoints();


	// Do the intitial weight generation
	vtkSmartPointer<vtkDeformPointSet> deform =
			vtkSmartPointer<vtkDeformPointSet>::New();
	deform->SetInputData(input);
	deform->SetControlMeshData(pd);
	deform->Update(); // this creates the initial weights

	// Display the warped polydata
	vtkSmartPointer<vtkPolyDataMapper> polyMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
	polyMapper->SetInputConnection(deform->GetOutputPort());
	vtkSmartPointer<vtkActor> polyActor =
			vtkSmartPointer<vtkActor>::New();
	polyActor->SetMapper(polyMapper);

	vtkSmartPointer<vtkJPEGReader> jpgReader = vtkSmartPointer<vtkJPEGReader>::New();
	jpgReader->SetFileName("texture.jpg");
	jpgReader->Update();

	vtkSmartPointer<vtkTexture> polyTexture = vtkSmartPointer<vtkTexture>::New();
	polyTexture->SetInputConnection(jpgReader->GetOutputPort());
	polyTexture->InterpolateOn();
	polyActor->SetTexture(polyTexture);

	// Display the warped polydata
	vtkSmartPointer<vtkPolyDataMapper> meshMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
	meshMapper->SetInputData(pd);

	vtkSmartPointer<vtkActor> meshActor =
			vtkSmartPointer<vtkActor>::New();
	meshActor->SetMapper(meshMapper);

	meshActor->GetProperty()->SetRepresentationToWireframe();



	renderer->AddActor(polyActor);
	renderer->AddActor(meshActor);

	renderer->GetActiveCamera()->SetPosition(-1, 1, -1);
	renderer->ResetCamera();
	renderer->SetBackground(.2, .3, .4);


	renWin->SetSize(1280, 720);

	const int FRAME = 30;
	for (int i = 0; i < FRAME; ++i) {
		// Now move one point and deform
		double controlPoint[3];
		pts->GetPoint(6, controlPoint);
		pts->SetPoint(6, controlPoint[0] - 7.f/FRAME,
					  controlPoint[1],
					  controlPoint[2] + 7.f/FRAME);
		pts->GetPoint(73, controlPoint);
		pts->SetPoint(73, controlPoint[0] + 10.f/FRAME,
					  controlPoint[1],
					  controlPoint[2]);
		pts->Modified();

		renWin->Render();

		vtkSmartPointer<vtkWindowToImageFilter> zBufferFilter =
				vtkSmartPointer<vtkWindowToImageFilter>::New();
		vtkSmartPointer<vtkBMPWriter> imageWriter =
				vtkSmartPointer<vtkBMPWriter>::New();
		vtkSmartPointer<vtkImageShiftScale> scale =
				vtkSmartPointer<vtkImageShiftScale>::New();

		// Create Depth Map
		zBufferFilter->SetInput(renWin);
//  zBufferFilter->MagniSetMagnification(1);
		zBufferFilter->SetInputBufferTypeToZBuffer();        //Extract z buffer value

		scale->SetOutputScalarTypeToUnsignedChar();
		scale->SetInputConnection(zBufferFilter->GetOutputPort());
		scale->SetShift(0);
		scale->SetScale(-255);

		// Write depth map as a .bmp image
		std::stringstream zBufferFilename;
		zBufferFilename << "iamgroot_zbuffer_" << i << ".bmp";
		imageWriter->SetFileName(zBufferFilename.str().c_str());
		imageWriter->SetInputConnection(scale->GetOutputPort());
		imageWriter->Write();

		vtkSmartPointer<vtkWindowToImageFilter> RGBFilter =	vtkSmartPointer<vtkWindowToImageFilter>::New();
		RGBFilter->SetInput(renWin);
		RGBFilter->SetInputBufferTypeToRGB(); // Extract RGB buffer value

		std::stringstream RGBFilename;
		RGBFilename << "iamgroot_RGB_" << i << ".bmp";
		imageWriter->SetFileName(RGBFilename.str().c_str());
		imageWriter->SetInputConnection(RGBFilter->GetOutputPort());
		imageWriter->Write();

		vtkSmartPointer<vtkOBJExporter> OBJExporter = vtkSmartPointer<vtkOBJExporter>::New();
		OBJExporter->SetRenderWindow(renWin);
		OBJExporter->SetActiveRenderer(renderer);
		std::stringstream objFilename;
		objFilename << "iamgroot_" << i;
		OBJExporter->SetFilePrefix(objFilename.str().c_str());
		OBJExporter->Write();

		std::string line;
		objFilename << ".mtl";
		fstream mat(objFilename.str().c_str());
		mat.seekg(0, mat.end);
		mat << "map_Ka texture.jpg\nmap_Kd texture.jpg\n";
	}
  iren->Start();

	return EXIT_SUCCESS;
}

