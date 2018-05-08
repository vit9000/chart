#pragma once

#define NULL 0
#define NULL_PTR NULL

namespace SMART_POINTER
{

	template<class TypeT>
	class shared_ptr
	{
	public:
		shared_ptr() : mp_Pointer(NULL_PTR), mp_CounterLink(NULL_PTR) {}
	/*	shared_ptr(const TypeT* ptr);
		shared_ptr(TypeT* &ptr);
		shared_ptr(const shared_ptr& istok);
		~shared_ptr();
	public:
		void Release(); //��������� ����� ��������� �� �������� �������� 
						//���������� ��������� ���-�� ������ �� ������. 
						//���� ���-�� ������ �� ������ ������ ������ ����, 
						//������ ����� ���������. 

		unsigned int IsEmpty(); //����� 1 - ���� ��������� ������. ��� 0 - ���� ���-�� ������

		unsigned int GetCounterLink(); //����� ���������� ������� ��������, ������� �� ���� ���������

		void Input(const shared_ptr& Istok); //������� ���������� ����� �������� �������� �� ������� ������ ���������
												 //������� ������ �����������. �������� �� ���������

		void Input(const TypeT* Object);

		void Input(TypeT* &Object); //������� ����� �������� ��������. 
									//�������� - ���������, ������� � ����� �������� ���������� (������ �� ����� ������� ��������)

		void Pass(shared_ptr &Priemnick); //�������� ����� �������� ������� ������ ���������
											  //������� ������ ����������. �������� �� ���������

		void PassAndForgot(shared_ptr &Priemnick); //�������� ������� ������ ��������� ����� �������� ��������, 
													   //����� �������� ����� ��������, �������� ��������� ���������.

		void InputAndForgot(shared_ptr &Istok);    //������� ����� ��������� ��������. �������� ���������

		TypeT* operator -> ();  //�������� ������ � ���������� ����������� �������.
								//������� �������� ������ � ��������������� ������� ����������� ������� � ��������.

		const TypeT* operator -> ()const;

		TypeT& operator* ();    //����� ������ �� ���������� ������

		shared_ptr& operator= (const shared_ptr& Istok);
		shared_ptr& operator= (TypeT* &Istok);
		shared_ptr& operator=(const TypeT* Object);
		*/

		shared_ptr(const TypeT* ptr) 
			: mp_Pointer(NULL_PTR), mp_CounterLink(NULL_PTR)
		{
			Input(ptr);
		}


		//shared_ptr(TypeT* &ptr) : mp_Pointer(NULL_PTR), mp_CounterLink(NULL_PTR) { Input(ptr); }


		//shared_ptr(const shared_ptr& Istok) : mp_Pointer(NULL_PTR), mp_CounterLink(NULL_PTR) { Input(Istok); }


		~shared_ptr() { Release(); }//��������� ����� ��������� �� �������� �������� 
						//���������� ��������� ���-�� ������ �� ������. 
						//���� ���-�� ������ �� ������ ������ ������ ����, 
						//������ ����� ���������. 


		void Release()
		{
			if (mp_CounterLink == 0) { return; }
			--(*mp_CounterLink);
			if ((*mp_CounterLink) == 0)
			{
				delete mp_CounterLink; delete mp_Pointer;
			}
			Clear();
		}



		void Input(const TypeT* pObject)
		{
			if (pObject == 0) { Release(); return; }

			/*ASSERT_ME((mp_Pointer != pObject),
				"void_shared_ptr::Input(const TypeT* pObject)",
				"������! ����� ��������� ��� �������� ������ � ����� �� �������, ��� � ���������. ���������� �� ����������� ������ �� ������� ������������");
			*/
			Release();
			mp_Pointer = const_cast<TypeT*>(pObject);
			mp_CounterLink = new int(1);
		}


		void Input(TypeT * &pObject)
		{
			if (pObject == 0) { Release(); return; }
			Release(); mp_Pointer = pObject; mp_CounterLink = new int(1);  pObject = NULL_PTR;
		}


		void Input(const shared_ptr& Istok)
		{
			if (mp_Pointer == Istok.mp_Pointer) { return; }
			Release();
			if (Istok.mp_Pointer == 0) { return; }
			mp_Pointer = Istok.mp_Pointer;  mp_CounterLink = Istok.mp_CounterLink;  (*mp_CounterLink)++;
		}


		void Pass(shared_ptr& Priemnick)
		{
			if (mp_Pointer == Priemnick.mp_Pointer) { return; }
			Priemnick.Release();
			if (mp_Pointer == 0) { return; }

			Priemnick.mp_Pointer = mp_Pointer;   Priemnick.mp_CounterLink = mp_CounterLink;   (*mp_CounterLink)++;
		}


		void PassAndForgot(shared_ptr& Priemnick) { Pass(Priemnick);    Release(); }


		void InputAndForgot(shared_ptr& Istok) { Input(Istok);    Istok.Release(); }

		operator bool() const
		{
			return IsEmpty();
		}

		operator TypeT*() const
		{
			return mp_Pointer;
		}

		TypeT* operator -> ()
		{
			/*CHECK_NULL_PTR_EXIT(mp_Pointer,
				"ptr_Type_shared_ptr::operator -> ()", "������! ������� �������� ������ �� �������� ���������");*/
			return mp_Pointer;
		}


		const TypeT* operator -> ()const
		{
			//CHECK_NULL_PTR_EXIT(mp_Pointer, "ptr_Type_shared_ptr::operator -> ()", "������! ������� �������� ������ �� �������� ���������");
			return mp_Pointer;
		}


		TypeT& shared_ptr::operator* ()
		{
			//ASSERT_MESSAGE(WORK_EXIT, (mp_Pointer != NULL_PTR),"ref_Type_shared_ptr::operator* ()", "������! ������� �������� ������ �� �������� ���������");
			return *mp_Pointer;
		}

		bool IsEmpty() const { return(mp_Pointer == 0); }

		unsigned int GetCounterLink() { return *mp_CounterLink; }

		shared_ptr<TypeT>&  operator= (const shared_ptr& Istok) { Input(Istok);  return *this; }

		shared_ptr<TypeT>&  operator= (TypeT* &Istok) { Input(Istok);  return *this; }

		shared_ptr<TypeT>&  operator=(const TypeT* Object) { Input(Object); return *this; }
	protected:
		TypeT* mp_Pointer;   //��������� ��������� �������
		int* mp_CounterLink; //����� �������� ���������� ������ �� ���������� ������
	private:
		void Clear() { mp_Pointer = 0;    mp_CounterLink = 0; }
	};



	

}